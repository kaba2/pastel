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

	template <typename IntervalSequence>
	class ToIndexSequence
	{
	};

	template <typename Type, int N>
	class ToIndexSequence<Vector<Type, N>>
	{
	public:
		using type = Vector<integer, N>;
	};

	//! Finds the nearest neighbors of a point in a PointKdTree.
	/*!
	kdTree:
	The kd-tree to search neighbors in. 
	Either a PointKdTree or a TdTree.

	searchPoint:
	The point for which to search a neighbor for.
	This can be either a Vector<Real, N>, or
	a Point_ConstIterator of 'kdTree'.

	timeIntervalSequence:
	An interval sequence in time. A sequence 
	(t_1, t_2, t_3, t_4, ...) corresponds to the
	time-intervals [t_1, t_2), [t_3, t_4), ...
	If the number of time-instants is odd, then
	the sequence is implicitly appended 
	infinity<Real>().

	nearestOutput:
	A reporter to which the found neighbors 
	(KdTree::Point_ConstIterator) are reported to.
	The reporting is done in the form
	nearestOutput(distance, point).

	acceptPoint:
	An indicator which decides whether to accept a point 
	(KdTree::Point_ConstIterator) as a neighbor or not.

	normBijection:
	The norm used to measure distance.

	searchAlgorithm:
	The search searchAlgorithm to use for searching the 'kdTree'.
	See 'pointkdtree_searchsearchAlgorithm.txt'.

	Optional arguments
	------------------

	maxDistance (Real >= 0):
	The distance after which points are not considered neighbors
	anymore. Can be set to infinity<Real>(). This distance
	is in terms of the used norm bijection.
	Default: infinity<Real>()

	maxRelativeError (Real >= 0):
	Maximum allowed relative error in the distance of the  
	result point to the true nearest neighbor. Allowing error
	increases performance. Use 0 for exact matches. 
	Default: 0

	nBruteForce (integer >= 0):
	The number of points under which to start a brute-force
	search in a node. Leaf nodes will always be searched.
	Default: 16

	kNearest (integer >= 0):
	The number of nearest neighbors to search.

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
		typename NearestOutput = Null_Output,
		typename Indicator = All_Indicator,
		typename Locator = typename KdTree::Locator,
		typename Real = Locator_Real<Locator>,
		typename NormBijection = Euclidean_NormBijection<Real>, 
		typename SearchAlgorithm_KdTree = DepthFirst_SearchAlgorithm_PointKdTree,
		typename IntervalSequence = Vector<Real, 2>,
		typename SetOptionals = Null_Output,
		Requires<
			// Visual Studio 2015 RC has bugs which cause these
			// commented stuff to fail.
			Or<
				IsPointKdTree<KdTree>,
				IsTdTree<KdTree>
			>,
			Models<NormBijection, NormBijection_Concept>,
			Models<Search_Point, Point_Concept>,
			Models<Indicator, Indicator_Concept(typename KdTree::Point_ConstIterator)>
			//Models<NearestOutput, Output_Concept(?)>,
		> ConceptCheck = 0
	>
	auto searchNearest(
		const KdTree& kdTree,
		const Search_Point& searchPoint,
		const NearestOutput& nearestOutput = NearestOutput(),
		const Indicator& acceptPoint = Indicator(),
		const NormBijection& normBijection = NormBijection(),
		const SearchAlgorithm_KdTree& searchAlgorithm_ = SearchAlgorithm_KdTree(),
		const IntervalSequence& timeIntervalSequence = IntervalSequence({-infinity<Real>(), infinity<Real>()}),
		SetOptionals setOptionals = SetOptionals())
	{
		using Locator = TdTree_Locator<KdTree>;
		using Real = Locator_Real<Locator>;
		using Fwd = KdTree;
		PASTEL_FWD(Cursor);
		PASTEL_FWD(Point_ConstIterator);

		using IndexSequence = 
			typename ToIndexSequence<IntervalSequence>::type;

		struct Optionals
		{
			integer k = 1;
			Real maxDistance2 = infinity<Real>();
			Real maxRelativeError = 0;
			integer nBruteForce = 16;
			bool reportMissing = true;
		} o;

		setOptionals(o);

		const integer& k = o.k;
		const Real& maxDistance2 = o.maxDistance2;
		const Real& maxRelativeError = o.maxRelativeError;
		const integer& nBruteForce = o.nBruteForce;
		const bool& reportMissing = o.reportMissing;

		ENSURE_OP(k, >=, 0);
		ENSURE_OP(maxDistance2, >=, 0);
		ENSURE_OP(maxRelativeError, >=, 0);
		ENSURE_OP(nBruteForce, >=, 0);

		const auto& locator = kdTree.locator();
		Real protectiveFactor = normBijection.scalingFactor(1.01);
		Real cullDistance2 = maxDistance2;
		Real errorFactor = inverse(normBijection.scalingFactor(1 + maxRelativeError));
		Real nodeCullDistance2 = cullDistance2 * errorFactor;
		integer dimension = kdTree.n();

		using Result = std::pair<Real, Point_ConstIterator>;
		Result notFound(infinity<Real>(), kdTree.end());

		if (k == 0 ||
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

        // The temporal restriction is given as a union
        // of time-intervals. Convert the time-points to
        // indices in the point-set.
		IndexSequence indexSequence(timeIntervalSequence);
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
			if (resultSet.size() == k)
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
			ASSERT_OP(resultSet.size(), <, k)

			// Add the new candidate.
			resultSet.emplace(distance2, iter);

			if (resultSet.size() == k)
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
			const IndexSequence& indexSequence = state.indexSequence;

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

				auto pointSet = cursor.pointSetAsInput(indexMin, indexMax);
				while (!pointSet.empty())
				{
					// Get the next node-point.
					Point_ConstIterator iter = pointSet.get();
					pointSet.pop();

					// Compute the distance from the node-point
					// to the search-point.
					Real currentDistance2 = 
						distance2(
							location(iter->point(), locator),
							searchPoint,
							normBijection,
							keepGoing
						);

					// Reject the point if the user rejects it or if we
					// already know the point cannot be among k nearest
					// neighbors. Remember that we are using an open 
					// search ball.
					if (currentDistance2 >= cullDistance2 || !acceptPoint(iter))
					{
						continue;
					}

					// Note that if there are multiple points at the same 
					// distance, then the points after the first should _not_
					// be culled away. We deal with this by expanding the 
					// suggested culling radius by a protective factor.
					Real cullSuggestion2 =
						considerPoint(currentDistance2, iter) * protectiveFactor;
					if (cullSuggestion2 < cullDistance2)
					{
						// The cull-radius got smaller; update it.
						cullDistance2 = cullSuggestion2;
						nodeCullDistance2 = cullDistance2 * errorFactor;
					}
				}
			}
        };

		typedef typename SearchAlgorithm_KdTree::template Instance<State>
			SearchAlgorithm;

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
		ASSERT_OP(neighbors, <=, k);

		// Report the nearest neighbors.
		for (auto&& entry : resultSet)
		{
			nearestOutput(entry.first, entry.second);
		}

		if (reportMissing)
		{
			// Report "not found" for the missing neighbors.
			for (integer i = neighbors;i < k;++i)
			{
				nearestOutput(notFound.first, notFound.second);
			}
		}

		if (neighbors < k)
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
