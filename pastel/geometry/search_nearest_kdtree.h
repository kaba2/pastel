// Description: Nearest neighbors searching in a kd-tree
// Documentation: nearestset.txt

#ifndef PASTELGEOMETRY_SEARCH_NEAREST_KDTREE_H
#define PASTELGEOMETRY_SEARCH_NEAREST_KDTREE_H

// Template concepts

#include "pastel/sys/indicator/indicator_concept.h"
#include "pastel/sys/output/output_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/math/normbijection/normbijection_concept.h"

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

namespace Pastel
{

	//! Finds the nearest neighbors of a point in a PointKdTree.
	/*!
	kdTree:
	The kd-tree to search neighbors in. 
	Either a PointKdTree or a TdTree.

	searchPoint:
	The point for which to search a neighbor for.
	This can be either a Vector<Real, N>, or
	a Point_ConstIterator of 'kdTree'.

	Optional arguments
	------------------

	accept:
	An indicator which decides whether to accept a point 
	(KdTree::Point_ConstIterator) as a neighbor or not.

	kNearest (integer >= 0):
	The number of nearest neighbors to search.

	maxDistance2 (Real >= 0):
	The distance after which points are not considered neighbors
	anymore. Can be set to infinity<Real>(). This distance
	is in terms of the used norm bijection.
	Default: infinity<Real>()

	maxRelativeError (Real >= 0):
	Maximum allowed relative error in the distance of the  
	result point to the true nearest neighbor. Allowing error
	increases performance. Use 0 for exact matches. 
	Default: 0

	report:
	A reporter to which the found neighbors 
	(KdTree::Point_ConstIterator) are reported to.
	The reporting is done in the form
	report(distance, point).

	nBruteForce (integer >= 0):
	The number of points under which to start a brute-force
	search in a node. Leaf nodes will always be searched.
	Default: 16

	normBijection:
	The norm used to measure distance.

	searchAlgorithm:
	The search searchAlgorithm to use for searching the 'kdTree'.
	See 'pointkdtree_searchsearchAlgorithm.txt'.

	timeIntervalSequence:
	An interval sequence in time. A sequence 
	(t_1, t_2, t_3, t_4, ...) corresponds to the
	time-intervals [t_1, t_2), [t_3, t_4), ...
	If the number of time-instants is odd, then
	the sequence is implicitly appended 
	infinity<Real>().

	returns (std::pair<Real, Point_ConstIterator>)
	----------------------------------------------
	
	The first element is the distance 
	(in terms of the norm bijection) to the k:th 
	nearest neighbor. If the k:th nearest neighbor 
	does not exist, infinity<Real>().
	
	The second element is the k:th nearest neighbor. 
	If the k:th nearest neighbor does not exist, kdTree.end().
	*/
	template <
		typename KdTree,
		typename Search_Point,
		typename... ArgumentSet,
		typename Locator = typename KdTree::Locator,
		typename Real = Locator_Real<Locator>,
		Requires<
			Or<
				IsPointKdTree<KdTree>,
				IsTdTree<KdTree>
			>,
			Models<Search_Point, Point_Concept>
		> ConceptCheck = 0
	>
	auto searchNearest(
		const KdTree& kdTree,
		const Search_Point& searchPoint,
		ArgumentSet&&... argumentSet)
	{
		using Fwd = KdTree;
		PASTEL_FWD(Cursor);
		PASTEL_FWD(Point_ConstIterator);

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
						Indicator_Concept(Point_ConstIterator)>();
				}
			);

		auto&& normBijection = 
			PASTEL_ARG(
				normBijection, 
				[]() {return Euclidean_NormBijection<real>();},
				[](auto input) {return implicitArgument(Models<decltype(input), NormBijection_Concept>());}
			);
		
		auto&& timeIntervalSequence = 
			PASTEL_ARG(
				intervalSequence, 
				[]() {return Vector<Real, 2>({-infinity<Real>(), infinity<Real>()});},
				[](auto input) {return Models<decltype(input), Point_Concept>();}
			);

		auto&& searchAlgorithmObject =
			PASTEL_ARG(
				searchAlgorithm, 
				[]() {return DepthFirst_SearchAlgorithm_PointKdTree();},
				[](auto input) {return std::true_type();}
			);

		integer kNearest = PASTEL_ARG_S(kNearest, 1);
		Real maxDistance2 = PASTEL_ARG_S(maxDistance2, infinity<Real>());
		Real maxRelativeError = PASTEL_ARG_S(maxRelativeError, 0);
		integer nBruteForce = PASTEL_ARG_S(nBruteForce, 16);
		bool reportMissing = PASTEL_ARG_S(reportMissing, false);

		ENSURE_OP(kNearest, >=, 0);
		ENSURE_OP(maxDistance2, >=, 0);
		ENSURE_OP(maxRelativeError, >=, 0);
		ENSURE_OP(nBruteForce, >=, 0);

		const auto& locator = kdTree.locator();
		Real protectiveFactor = normBijection.scalingFactor(1.01);
		Real cullDistance2 = maxDistance2;
		Real errorFactor = inverse(normBijection.scalingFactor(1 + maxRelativeError));
		Real nodeCullDistance2 = cullDistance2 * errorFactor;

		using Result = std::pair<Real, Point_ConstIterator>;
		Result notFound(infinity<Real>(), kdTree.end());

		if (kNearest == 0 ||
			kdTree.empty() ||
			maxDistance2 == 0)
		{
			// There is nothing to search for.
			// Note that we consider the search-ball open.
			return notFound;
		}

		// Compute the distance from the search-point to the
		// bounding-box of the kd-tree.
		Real rootDistance2 = 
			distance2(kdTree.bound(), searchPoint, normBijection);
		if (rootDistance2 >= maxDistance2)
		{
			// The bounding box for the points does not
			// intersect the search ball.
			return notFound;
		}

		using TimeSequence = 
			RemoveCvRef<decltype(timeIntervalSequence)>;
		using IndexSequence = 
			Vector<integer, Point_N<TimeSequence>::value>;

		// The temporal restriction is given as a union
        // of time-intervals. Convert the time-points to
        // indices in the point-set.
		IndexSequence indexSequence(
			ofDimension(timeIntervalSequence.n()));
		for (integer i = 0;i < timeIntervalSequence.size();i += 2)
		{
			indexSequence[i] = kdTree.timeToIndex(
				timeIntervalSequence[i]);
			
			if (i + 1 < timeIntervalSequence.size())
			{
				indexSequence[i + 1] = kdTree.timeToIndex(
					timeIntervalSequence[i + 1]);
			}
		}

		// This set contains the points currently closest
		// to the search-point. There will be at most
		// k elements in this set.
		using ResultSet = std::set<Result>;
		ResultSet resultSet;

		auto considerPoint = [&](
			const Real& distance2,
			const Point_ConstIterator& iter)
		{
			if (resultSet.size() == kNearest)
			{
				// There are k candidates already.

				Real farthestDistance2 = 
					resultSet.empty() ? 
					infinity<Real>() :
					std::prev(resultSet.end())->first;
				if (distance2 < farthestDistance2)
				{
					// The new candidate is closer
					// than the farthest candidate.

					// Remove the farthest candidate.
					resultSet.erase(
						std::prev(resultSet.end()));
				}
				else
				{
					// The new candidate is farther
					// than the farthest candidate.
					return farthestDistance2;
				}
			}

			// There are now less than k candidates
			// in the candidate set.
			ASSERT_OP(resultSet.size(), <, kNearest)

			// Add the new candidate.
			resultSet.emplace(distance2, iter);

			if (resultSet.size() == kNearest)
			{
				// Since the candidate set contains k
				// elements, everything beyond the
				// farthest candidate can be rejected.
				return std::prev(resultSet.end())->first;
			}

			// There are less than k candidate elements;
			// we cannot yet reject any points based
			// on distance.
			return infinity<Real>();
		};

		// This is the condition for keeping computing
		// the distance to the search-point component
		// by component, used by distance2().
		auto keepGoing = [&](const Real& that)
		{
			// Stop computing the distance if it exceeds
			// the culling distance.
			return that < cullDistance2;
		};

		struct State
		: boost::less_than_comparable<State>
		{
			State(
				const Cursor& cursor_,
				const Real& distance_,
				const IndexSequence& indexSequence_)
			: cursor(cursor_)
			, distance(distance_)
			, indexSequence(indexSequence_)
			{
			}

			bool operator<(const State& that) const
			{
				return distance < that.distance;
			}

			Cursor cursor;
			Real distance;
			IndexSequence indexSequence;
		};

        // This is used to search through the points in 
        // a node using brute-force.
		auto searchBruteForce = [&](const State& state)
        {
            // Each searched node is selected such that either
            // * the node is a leaf node, or
            // * the node has at most nBruteForce points.

            Cursor cursor = state.cursor;

            // Having const IndexSequence& here triggers a
            // bug in gcc 4.9.2. But const auto& works!
			const auto& indexSequence = state.indexSequence;

			for (integer i = 0; i < indexSequence.size(); i += 2)
			{
				// For each pair of integers in the index-sequence...

				// The index-sequence is a sequence of integer 
				// pairs (i, j). If the index-sequence has an odd
				// number of elements, then the last index is
				// implicitly taken to be infinity.
				integer indexMin = indexSequence[i];
				integer indexMax = (i + 1) < indexSequence.size() ?
					indexSequence[i + 1] : cursor.points();

				auto pointSet = cursor.pointSet(indexMin, indexMax);
				forEach(pointSet, [&](auto&& point)
				{
					// Compute the distance from the node-point
					// to the search-point.
					Real currentDistance2 = 
						distance2(
							location(point->point(), locator),
							searchPoint,
							normBijection,
							keepGoing
						);

					// Reject the point if the user rejects it or if we
					// already know the point cannot be among k nearest
					// neighbors. Remember that we are using an open 
					// search ball.
					if (currentDistance2 >= cullDistance2 || !accept(point))
					{
						return true;
					}

					// Note that if there are multiple points at the same 
					// distance, then the points after the first should _not_
					// be culled away. We deal with this by expanding the 
					// suggested culling radius by a protective factor.
					Real cullSuggestion2 =
						considerPoint(currentDistance2, point) * protectiveFactor;
					if (cullSuggestion2 < cullDistance2)
					{
						// The cull-radius got smaller; update it.
						cullDistance2 = cullSuggestion2;
						nodeCullDistance2 = cullDistance2 * errorFactor;
					}

					return true;
				});
			}
        };

		using SearchAlgorithm = 
			typename RemoveCvRef<decltype(searchAlgorithmObject)>::template Instance<State>;

		SearchAlgorithm searchAlgorithm;

		// Start from the root node.
		searchAlgorithm.insertNode(
			State(kdTree.root(), rootDistance2, indexSequence));

		while (searchAlgorithm.nodesLeft())
		{
			State state = searchAlgorithm.nextNode();
			
			const Real& distance = state.distance;
			const Cursor& cursor = state.cursor;
			const IndexSequence& intervalSequence = state.indexSequence;

			if (distance > nodeCullDistance2)
			{
				if (searchAlgorithm.breakOnCulling())
				{
					break;
				}
				else
				{
					continue;
				}
			}

			// Search a node with brute-force if it is a leaf node, or the 
			// search-algorithm says so. Usually the latter is when 
			// cursor.points() <= nBruteForce.
			if (cursor.leaf() ||
				searchAlgorithm.shouldSearchSplitNode(state, nBruteForce))
			{
				searchBruteForce(state);
				continue;
			}

			// For an intermediate node our task is to
			// recurse to child nodes while updating
			// incrementally the distance 
			// to the current node. 

			integer splitAxis = cursor.splitAxis();
			Real searchPosition = 
				pointAxis(searchPoint, splitAxis);

			Cursor left = cursor.left();
			Cursor right = cursor.right();

			// Compute the distances to the boundary 
			// planes of the child nodes.

			Real oldAxisDistance = 0;
			Real leftAxisDistance = 0;
			Real rightAxisDistance = 0;
			if (searchPosition < right.min())
			{
				rightAxisDistance = 
					normBijection.axis(right.min() - searchPosition);
				if (searchPosition < left.min())
				{
					leftAxisDistance = 
						normBijection.axis(left.min() - searchPosition);
					if (searchPosition < left.prevMin())
					{
						oldAxisDistance = 
							normBijection.axis(left.prevMin() - searchPosition);
					}
				}
			}
			if (searchPosition > left.max())
			{
				leftAxisDistance = 
					normBijection.axis(searchPosition - left.max());
				if (searchPosition > right.max())
				{
					rightAxisDistance = 
						normBijection.axis(searchPosition - right.max());
					if (searchPosition > right.prevMax())
					{
						oldAxisDistance = 
							normBijection.axis(searchPosition - right.prevMax());
					}
				}
			}
			
			// Compute the actual distances
			// to the child nodes. These are
			// only done if the nodes can't be
			// culled by the boundary plane distance.

			Real leftDistance = leftAxisDistance;
			if (leftAxisDistance <= nodeCullDistance2)
			{
				leftDistance = 
					normBijection.replaceAxis(
					distance,
					oldAxisDistance,
					leftAxisDistance);
			}

			Real rightDistance = rightAxisDistance; 
			if (rightAxisDistance <= nodeCullDistance2)
			{
				rightDistance = 
					normBijection.replaceAxis(
					distance,
					oldAxisDistance,
					rightAxisDistance);
			}

			// Queue non-culled child nodes for 
			// future handling.

			IndexSequence leftSequence(intervalSequence);
			IndexSequence rightSequence(intervalSequence);
			for (integer i = 0;i < intervalSequence.size();++i)
			{
				leftSequence[i] = cursor.cascade(intervalSequence[i], false);
				rightSequence[i] = cursor.cascade(intervalSequence[i], true);
			}

			State leftState(left, leftDistance, leftSequence);
			State rightState(right, rightDistance, rightSequence);

			if (leftDistance <= nodeCullDistance2 &&
				!searchAlgorithm.skipNode(leftState))
			{
				if (rightDistance <= nodeCullDistance2 && 
					!searchAlgorithm.skipNode(rightState))
				{
					searchAlgorithm.insertNodes(
						leftState,
						rightState);
				}
				else
				{
					searchAlgorithm.insertNode(leftState);
				}
			}
			else if (rightDistance <= nodeCullDistance2 && 
				!searchAlgorithm.skipNode(rightState))
			{
				searchAlgorithm.insertNode(rightState);
			}
		}

		// There should be at most k neighbors.
		integer neighbors = resultSet.size();
		ASSERT_OP(neighbors, <=, kNearest);

		// Report the nearest neighbors.
		for (auto&& entry : resultSet)
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

		if (neighbors < kNearest)
		{
			// Since there are less than k neighbors,
			// the k:th nearest neighbor does not
			// exist.
			return notFound;
		}

		// Return the k:th nearest neighbor.
		return *std::prev(resultSet.end());
	}

}

#endif
