// Description: Nearest neighbors searching in a kd-tree
// Documentation: nearestset.txt

#ifndef PASTELGEOMETRY_SEARCH_NEAREST_H
#define PASTELGEOMETRY_SEARCH_NEAREST_H

// Template concepts

#include "pastel/sys/indicator/indicator_concept.h"
#include "pastel/sys/output/output_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/math/normbijection/normbijection_concept.h"
#include "pastel/geometry/nearestset/nearestset_concept.h"

// Template defaults

#include "pastel/math/normbijection/euclidean_normbijection.h"
#include "pastel/sys/indicator/all_indicator.h"
#include "pastel/sys/output/null_output.h"
#include "pastel/geometry/depthfirst_pointkdtree_searchalgorithm.h"

// Template requirements

#include "pastel/sys/type_traits/is_template_instance.h"
#include "pastel/geometry/pointkdtree/pointkdtree_fwd.h"
#include "pastel/geometry/tdtree/tdtree_fwd.h"

// Implementation requirements

#include "pastel/geometry/distance/distance_point_point.h"
#include "pastel/geometry/distance/distance_alignedbox_point.h"

#include "pastel/sys/rankedset/rankedset.h"

namespace Pastel
{

	//! Finds the nearest neighbors of a point in a PointKdTree.
	/*!
	kdTree:
	The kd-tree to search neighbors in. 
	Either a PointKdTree or a TdTree.

	searchPoint (Point):
	The point for which to search a neighbor for.

	Optional arguments
	------------------

	accept (Indicator(KdTree::Point_ConstIterator)):
	An indicator which decides whether to accept a point 
	as a neighbor or not.

	kNearest (integer >= 0):
	The number of nearest neighbors to search.

	maxDistance2 (Real >= 0):
	The distance after which points are not considered neighbors
	anymore. Can be set to (Real)Infinity(). This distance
	is in terms of the used norm bijection.
	Default: (Real)Infinity()

	maxRelativeError (Real >= 0):
	Maximum allowed relative error in the distance of the 
	result point to the true nearest neighbor. Allowing error
	increases performance. Use 0 for exact matches. 
	Default: 0

	report (Output(KdTree::Point_ConstIterator)):
	An output to which the found neighbors 
	are reported to. The reporting is done in the 
	form report(distance, point), in decreasing
	order of distance.

	reportMissing (bool):
	Whether to always report kNearest points, even
	if there are not so many neighbors. The missing 
	points are reported as (Infinity(), kdTree.end()).
	Default: true

	nBruteForce (integer >= 0):
	The number of points under which to start a brute-force
	search in a node. Leaf nodes will always be searched.
	Default: 16

	normBijection:
	The norm used to measure distance.

	searchAlgorithm:
	The search-algorithm to use for searching the 'kdTree'.
	See 'pointkdtree_searchalgorithm.txt'.

	timeIntervalSequence:
	An interval sequence in time. A sequence 
	(t_1, t_2, t_3, t_4, ...) corresponds to the
	time-intervals [t_1, t_2), [t_3, t_4), ...
	If the number of time-instants is odd, then
	the sequence is implicitly appended 
	(Real)Infinity().

	returns (std::pair<Real, Point_ConstIterator>)
	----------------------------------------------
	
	The first element is the distance 
	(in terms of the norm bijection) to the k:th 
	nearest neighbor. If the k:th nearest neighbor 
	does not exist, (Real)Infinity().
	
	The second element is the k:th nearest neighbor. 
	If the k:th nearest neighbor does not exist, kdTree.end().
	*/
	template <
		typename NearestSet,
		typename Search_Point,
		typename... ArgumentSet,
		Requires<
			Models<NearestSet, NearestSet_Concept>,
			Models<Search_Point, Point_Concept>
		> = 0
	>
	auto searchNearest(
		const NearestSet& nearestSet,
		const Search_Point& searchPoint,
		ArgumentSet&&... argumentSet)
	{
		using std::begin;

		using PointId = RemoveCvRef<decltype(*begin(nearestSet.pointSet()))>;
		using Real = Point_Real<Search_Point>;

		auto&& report = 
			PASTEL_ARG(
				report, 
				[]() {return nullOutput();},
				[](auto input) {return std::true_type();}
			);

		auto&& accept = 
			PASTEL_ARG(
				accept, 
				[]() {return allIndicator();},
				[](auto input) 
				{
					return Models<decltype(input), 
						Indicator_Concept(PointId)>();
				}
			);

		auto&& normBijection = 
			PASTEL_ARG(
				normBijection, 
				[]() {return Euclidean_NormBijection<real>();},
				[](auto input) {return implicitArgument(Models<decltype(input), NormBijection_Concept>());}
			);
		
		integer kNearest = PASTEL_ARG_S(kNearest, 1);
		Real maxDistance2 = PASTEL_ARG_S(maxDistance2, (Real)Infinity());
		bool reportMissing = PASTEL_ARG_S(reportMissing, false);

		ENSURE_OP(kNearest, >=, 0);
		ENSURE_OP(maxDistance2, >=, 0);

		using Result = std::pair<Real, PointId>;
		Result notFound((Real)Infinity(), *begin(nearestSet.pointSet()));

		if (kNearest == 0 || maxDistance2 == 0)
		{
			// There is nothing to search for.
			// Note that we consider the search-ball open.
			return notFound;
		}

		const Real protectiveFactor = normBijection.scalingFactor(1.01);
		// The distance beyond which points are ignored.
		Real cullDistance2 = maxDistance2;

		struct Less
		{
			bool operator()(const Result& left, const Result& right) const
			{
				return left.first < right.first;
			}
		};

		// This set contains the points currently closest
		// to the search-point. 
		using ResultSet = RankedSet<Result, Less>;
		// There will be at most k elements in this set.
		ResultSet resultSet(kNearest);

		auto searchBruteForce = [&](auto&& pointIdSet)
		{
			for(auto&& pointId : pointIdSet)
			{
				// Compute the distance from the node-point
				// to the search-point.
				Real currentDistance2 = 
					distance2(
						nearestSet.asPoint(pointId),
						searchPoint,
						PASTEL_TAG(normBijection),
						normBijection,
						PASTEL_TAG(keepGoing),
						// Stop computing the distance if it exceeds
						// the culling distance.
						[&](auto&& that) {return that < cullDistance2;}
					);

				// Reject the point if the user rejects it or if we
				// already know the point cannot be among k nearest
				// neighbors. Remember that we are using an open 
				// search ball.
				if (currentDistance2 >= cullDistance2 || !accept(pointId))
				{
					continue;
				}

				// Add the new candidate.
				resultSet.push(Result(currentDistance2, pointId));

				Real maxNearestDistance2 = (Real)Infinity();
				if (resultSet.full())
				{
					// Since the candidate set contains k
					// elements, everything beyond the
					// farthest candidate can be rejected.
					maxNearestDistance2 = resultSet.top().first;
				}

				// Note that if there are multiple points at the same 
				// distance, then the points after the first should _not_
				// be culled away. We deal with this by expanding the 
				// suggested culling radius by a protective factor.
				const Real cullSuggestion2 =
					maxNearestDistance2 * protectiveFactor;
				if (cullSuggestion2 < cullDistance2)
				{
					// The cull-radius got smaller; update it.
					cullDistance2 = cullSuggestion2;
				}
			}
		};

		nearestSet.nearbyPointSetSet(
			searchPoint,
			normBijection,
			cullDistance2,
			searchBruteForce);

		// Sort the neighbors in order of
		// increasing distance.
		auto sortedSet = resultSet.release();

		// Report the nearest neighbors.
		for (auto&& entry : sortedSet)
		{
			report(entry.first, entry.second);
		}

		// There should be at most k neighbors.
		integer neighbors = sortedSet.size();
		ASSERT_OP(neighbors, <=, kNearest);

		if (reportMissing)
		{
			// Report "not found" for the missing neighbors.
			for (integer i = neighbors;i < kNearest;++i)
			{
				report(notFound.first, notFound.second);
			}
		}

		if (neighbors < kNearest)
		{
			// Since there are less than k neighbors,
			// the k:th nearest neighbor does not
			// exist.
			return notFound;
		}

		// Return the k:th nearest neighbor.
		return sortedSet.back();
	}

}

#endif
