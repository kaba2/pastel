// Description: Nearest neighbors searching in a kd-tree
// Documentation: nearestset.txt

#ifndef PASTELGEOMETRY_KDTREE_NEARESTSET_H
#define PASTELGEOMETRY_KDTREE_NEARESTSET_H

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

#include "pastel/sys/rankedset/rankedset.h"
#include "pastel/sys/set/range_set.h"

#include <experimental/generator>

namespace Pastel
{

	template <
		typename KdTree,
		typename IntervalSequence,
		typename SearchAlgorithm>
	class KdTree_NearestSet
	{
	public:
		using Fwd = KdTree;
		PASTEL_FWD(Real);
		PASTEL_FWD(Cursor);
		PASTEL_FWD(Point_ConstIterator);
		using ConstIterator = Point_ConstIterator;

		KdTree_NearestSet(
			const KdTree& kdTree_,
			const Real& maxRelativeError_,
			integer nBruteForce_,
			const IntervalSequence& timeIntervalSequence_)
		: kdTree(kdTree_)
		, maxRelativeError(maxRelativeError_)
		, nBruteForce(nBruteForce_)
		, timeIntervalSequence(timeIntervalSequence_)
		{
		}

		auto pointSet() const
		{
			return intervalSet(kdTree.begin(), kdTree.end());
		}

		auto begin() const
		{
			using std::begin;
			return begin(pointSet());
		}

		auto end() const
		{
			using std::end;
			return end(pointSet());
		}

		const auto& pointSetLocator() const
		{
			return kdTree.locator();
		}

		decltype(auto) asPoint(Point_ConstIterator point) const
		{
			return location(point->point(), kdTree.locator());
		}

		template <
			typename Search_Point,
			typename NormBijection,
			Requires<
				Models<Search_Point, Point_Concept>,
				Models<NormBijection, NormBijection_Concept>
			> = 0
		>
		decltype(auto) nearbyPointSetSet(
			const Search_Point& searchPoint,
			const NormBijection& normBijection,
			const Real& cullDistance2) const
		{
			Real errorFactor = inverse(normBijection.scalingFactor(1 + maxRelativeError));
			Real nodeCullDistance2 = cullDistance2 * errorFactor;

			if (kdTree.empty())
			{
				// There is nothing to search for.
				// Note that we consider the search-ball open.
				co_return;
			}

			// Compute the distance from the search-point to the
			// bounding-box of the kd-tree.
			Real rootDistance2 = 
				distance2(kdTree.bound(), searchPoint, normBijection);
			if (rootDistance2 >= cullDistance2)
			{
				// The bounding box for the points does not
				// intersect the search ball.
				co_return;
			}

			using TimeSequence = 
				RemoveCvRef<decltype(timeIntervalSequence)>;
			using IntervalSequence = 
				Vector<integer, Point_N<TimeSequence>::value>;

			// The temporal restriction is given as a union
	        // of time-intervals. Convert the time-points to
	        // indices in the point-set.
			IntervalSequence indexSequence(
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

			struct State
			: boost::less_than_comparable<State>
			{
				State(
					const Cursor& cursor_,
					const Real& distance_,
					const IntervalSequence& indexSequence_)
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
				IntervalSequence indexSequence;
			};

			using SearchAlgorithm_ = 
				SearchAlgorithm::template Instance<State>;

			SearchAlgorithm_ searchAlgorithm;

			// Start from the root node.
			searchAlgorithm.insertNode(
				State(kdTree.root(), rootDistance2, indexSequence));

			while (searchAlgorithm.nodesLeft())
			{
				State state = searchAlgorithm.nextNode();
				
				const Real& distance = state.distance;
				const Cursor& cursor = state.cursor;
				const IntervalSequence& intervalSequence = state.indexSequence;

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
					// Search the node using brute-force.

		            Cursor cursor = state.cursor;

		            // Having const IntervalSequence& here triggers a
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

						// FIX: The following should be
						// co_yield cursor.pointSet(indexMin, indexMax);
						// But this triggers an internal compiler error
						// in Visual Studio 2017. The following avoids
						// that error:

						auto pointSet = rangeSet(
								cursor.pointSet(indexMin, indexMax).begin(),
								cursor.pointSet(indexMin, indexMax).end());

						co_yield pointSet;
					}

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

				IntervalSequence leftSequence(intervalSequence);
				IntervalSequence rightSequence(intervalSequence);
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

		}

		const KdTree& kdTree;
		Real maxRelativeError;
		integer nBruteForce;
		const IntervalSequence& timeIntervalSequence;
	};

	template <
		typename KdTree,
		typename... ArgumentSet>
	decltype(auto) kdTreeNearestSet(
		const KdTree& kdTree,
		ArgumentSet&&... argumentSet)
	{
		using Fwd = KdTree;
		PASTEL_FWD(Real);

		Real maxRelativeError = PASTEL_ARG_S(maxRelativeError, 0);
		ENSURE_OP(maxRelativeError, >=, 0);

		integer nBruteForce = PASTEL_ARG_S(nBruteForce, 16);
		ENSURE_OP(nBruteForce, >=, 0);

		auto&& timeIntervalSequence = 
			PASTEL_ARG(
				intervalSequence, 
				[]() {return Vector<Real, 2>({-(Real)Infinity(), (Real)Infinity()});},
				[](auto input) {return Models<decltype(input), Point_Concept>();}
			);
		using IntervalSequence = decltype(timeIntervalSequence);

		auto&& searchAlgorithmObject =
			PASTEL_ARG(
				searchAlgorithm, 
				[]() {return DepthFirst_SearchAlgorithm_PointKdTree();},
				[](auto input) {return std::true_type();}
			);
		using SearchAlgorithm = RemoveCvRef<decltype(searchAlgorithmObject)>;

		return KdTree_NearestSet<KdTree, IntervalSequence, SearchAlgorithm>(
			kdTree,
			maxRelativeError,
			nBruteForce,
			timeIntervalSequence);
	}

}

#endif
