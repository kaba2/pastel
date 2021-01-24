// Description: Nearest neighbors searching in a kd-tree
// Documentation: nearestset.txt

#ifndef PASTELGEOMETRY_SEARCH_NEAREST_H
#define PASTELGEOMETRY_SEARCH_NEAREST_H

// Template concepts

#include "pastel/sys/indicator/indicator_concept.h"
#include "pastel/sys/output/output_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/math/norm/norm_concept.h"
#include "pastel/geometry/nearestset/nearestset_concept.h"
#include "pastel/geometry/search_nearby.h"

// Template defaults

#include "pastel/math/norm/euclidean_norm.h"
#include "pastel/sys/indicator/all_indicator.h"
#include "pastel/sys/output/null_output.h"

// Implementation requirements

#include "pastel/sys/rankedset/rankedset.h"

namespace Pastel
{

	//! Finds the nearest neighbors of a point in a nearest-set.
	/*!
	nearestSet (NearestSet):
	The nearest-set in which to search neighbors in.

	searchPoint (Point):
	The point for which to search a neighbor for.

	Optional arguments
	------------------

	accept (Indicator(PointId)):
	An indicator which decides whether to accept a point 
	as a neighbor or not.
	Default: allIndicator()

	kNearest (integer >= 0):
	The number of nearest neighbors to search for.
	Default: 1

	maxDistance2 (Distance):
	The distance after which points are not considered neighbors
	anymore. Can be set to (Real)Infinity(). 
	Default: norm((Real)Infinity())

	sortDistances (bool):
	Whether to report the neighbors in increasing
	order of distance, as opposed to heap-order.
	Default: true

	report (Output(Real, PointId)):
	An output to which the found neighbors 
	are reported to. The reporting is done in the 
	form report(distance, point). 
	Default: nullOutput()

	reportMissing (bool):
	Whether to always report kNearest points, even
	if there are not so many neighbors. The missing 
	points are reported as (Infinity(), nearestSet.notFound()).
	Default: true

	norm (Norm):
	The norm used to measure distance.

	returns (std::pair<Real, PointId>)
	----------------------------------------------
	
	The first element is the distance 
	(in terms of the norm bijection) to the k:th 
	nearest neighbor. If the k:th nearest neighbor 
	does not exist, (Real)Infinity().
	
	The second element is the k:th nearest neighbor. 
	If the k:th nearest neighbor does not exist, nearestSet.notFound().
	*/
	template <
		NearestSet_Concept NearestSet,
		Point_Concept Search_Point,
		typename... ArgumentSet
	>
	auto searchNearest(
		const NearestSet& nearestSet,
		const Search_Point& searchPoint,
		ArgumentSet&&... argumentSet)
	{
		using std::begin;

		using PointId = PointSet_PointId<NearestSet>;
		using Real = Point_Real<Search_Point>;

		auto&& report = 
			PASTEL_ARG_C(report, nullOutput(), Trivial_Concept);

		auto&& accept = 
			PASTEL_ARG_C1(accept, allIndicator(), Indicator_Concept, PointId);

		auto&& norm = 
			PASTEL_ARG_C(norm, Euclidean_Norm<Real>(), Norm_Concept);

		using Distance = decltype(norm());

		integer kNearest = PASTEL_ARG_S(kNearest, 1);
		Distance maxDistance2 = PASTEL_ARG_S(maxDistance2, norm((Real)Infinity()));
		bool reportMissing = PASTEL_ARG_S(reportMissing, false);
		bool sortDistances = PASTEL_ARG_S(sortDistances, true);

		ENSURE_OP(kNearest, >=, 0);

		using Result = std::pair<Distance, PointId>;
		Result notFound(norm((Real)Infinity()), *begin(nearestSet.pointSet()));

		if (kNearest == 0 || ~maxDistance2 == 0)
		{
			// There is nothing to search for.
			// Note that we consider the search-ball open.
			return notFound;
		}

		struct Less
		{
			bool operator()(const Result& left, const Result& right) const
			{
				return left.first < right.first;
			}
		};

		// The number of points in the point-set.
		const integer n = nearestSet.points();

		// There cannot be more than n neighbors; 
		// avoid allocating storage in case kNearest
		// is excessive. If we are counting, we 
		// do not track any neighbor.
		const integer resultSetSize = std::min(kNearest, n);

		// This set contains the points currently closest
		// to the search-point. 
		using ResultSet = RankedSet<Result, Less>;
		// There will be at most k elements in this set.
		ResultSet resultSet(resultSetSize);

		auto reportCandidate = [&](
			const Distance& distance2,
			const auto& pointId)
		{
			resultSet.push(Result(distance2, pointId));
			if (resultSet.full())
			{
				return resultSet.top().first;
			}

			return Distance((Real)Infinity());
		};

		searchNearby(
			nearestSet,
			searchPoint,
			PASTEL_TAG(accept), accept,
			PASTEL_TAG(report), reportCandidate,
			PASTEL_TAG(norm), norm,
			PASTEL_TAG(maxDistance2), maxDistance2);

		// There should be at most k neighbors.
		const integer neighbors = resultSet.size();
		ASSERT_OP(neighbors, <=, kNearest);

		Result farthest = notFound;
		if (neighbors == kNearest)
		{
			// Get the k:th nearest neighbor;
			// it is at the top of the heap.
			farthest = resultSet.top();
		}

		// Perhaps sort the neighbors in 
		// order of increasing distance.
		auto reportSet = resultSet.release(sortDistances);

		// Report the nearest neighbors.
		for (auto&& entry : reportSet)
		{
			report(entry.first, entry.second);
		}

		if (reportMissing)
		{
			// Report "not found" for the missing neighbors.
			for (integer i = neighbors;i < kNearest;++i)
			{
				report(notFound.first, notFound.second);
			}
		}

		// Return the k:th nearest neighbor,
		// or the farthest neighbor when
		// counting.
		return farthest;
	}

}

#endif
