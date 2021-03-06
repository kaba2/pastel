// Description: Nearest neighbors searching in a kd-tree
// Documentation: nearestset.txt

#ifndef PASTELGEOMETRY_KDTREE_NEARESTSET_H
#define PASTELGEOMETRY_KDTREE_NEARESTSET_H

// Template concepts

#include "pastel/sys/indicator/indicator_concept.h"
#include "pastel/sys/output/output_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/math/norm/norm_concept.h"
#include "pastel/geometry/nearestset/nearestset_concept.h"

// Template defaults

#include "pastel/math/distance/euclidean_distance.h"
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

#include <optional>

namespace Pastel
{

	template <
		typename KdTree,
		typename IntervalSequence_,
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
			IntervalSequence_ timeIntervalSequence_)
		: kdTree(kdTree_)
		, maxRelativeError(maxRelativeError_)
		, nBruteForce(nBruteForce_)
		, timeIntervalSequence(std::move(timeIntervalSequence_))
		{
		}

		integer points() const {
			return kdTree.points();
		}

		auto pointSet() const
		{
			return intervalRange(kdTree.begin(), kdTree.end());
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
			Point_Concept Search_Point,
			Norm_Concept Norm,
			Distance_Concept Distance,
			typename Output
		>
		void findNearbyPointsets(
			const Search_Point& searchPoint,
			const Norm& norm,
			const Distance& maxDistance2,
			const Output& report) const
		{
			Distance cullDistance2 = maxDistance2;

			const Real errorFactor = inverse(1 + maxRelativeError);
			Distance nodeCullDistance2 = cullDistance2 * errorFactor;

			if (kdTree.empty())
			{
				// There is nothing to search for.
				// Note that we consider the search-ball open.
				return;
			}

			// Compute the distance from the search-point to the
			// bounding-box of the kd-tree.
			Distance rootDistance2 = 
				distance2(kdTree.bound(), searchPoint, PASTEL_TAG(norm), norm);
			if (rootDistance2 >= maxDistance2)
			{
				// The bounding box for the points does not
				// intersect the search ball.
				return;
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
					const Distance& distance_,
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
				Distance distance;
				IntervalSequence indexSequence;
			};

			using SearchAlgorithm_ = 
				typename SearchAlgorithm::template Instance<State>;

			SearchAlgorithm_ searchAlgorithm;

			// Start from the root node.
			searchAlgorithm.insertNode(
				State(kdTree.root(), rootDistance2, indexSequence));

			auto intervalDistance = [](
				const Real& x,
				const Real& min, 
				const Real& max)
			{
				if (x < min) return min - x;
				if (x > max) return x - max;
				return (Real)0;
			};

			auto queueNode = [&](
				const State& parent,
				const Real& searchPosition,
				bool right)
			{
				const integer axis = parent.cursor.splitAxis();
				const Cursor node = right ? parent.cursor.right() : parent.cursor.left();

				const Real axisDistance = 
					intervalDistance(
						searchPosition, node.min(), node.max());
				
				const Real axisDistancePrev = 
					intervalDistance(
						searchPosition, node.prevMin(), node.prevMax());

				auto newDistance2 = 
					Distance(parent.distance).replace(
						axis,
						axisDistancePrev,
						axisDistance);

				IntervalSequence sequence(parent.indexSequence);
				for (integer i = 0;i < parent.indexSequence.size();++i)
				{
					sequence[i] = parent.cursor.cascade(parent.indexSequence[i], right);
				}

				std::optional<State> maybeState;

				State state(node, newDistance2, sequence);

				if (newDistance2 <= nodeCullDistance2 &&
					!searchAlgorithm.skipNode(state))
				{
					maybeState = state;
				}

				return maybeState;
			};

			while (searchAlgorithm.nodesLeft())
			{
				State state = searchAlgorithm.nextNode();
				
				const auto& distance = state.distance;
				const Cursor& cursor = state.cursor;
				const IntervalSequence& intervalSequence = state.indexSequence;

				if (distance > nodeCullDistance2)
				{
					if (searchAlgorithm.breakOnCulling())
					{
						break;
					}

					continue;
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

						const Distance cullSuggestion2 = 
							report(
								cursor.pointSet(indexMin, indexMax), 
								cullDistance2);
						if (cullSuggestion2 < cullDistance2)
						{
							cullDistance2 = cullSuggestion2;
							nodeCullDistance2 = cullDistance2 * errorFactor;
						}
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

				// Queue non-culled child nodes for 
				// future handling.
				auto left = queueNode(state, searchPosition, false);
				auto right = queueNode(state, searchPosition, true);

				if (left && right) {
					searchAlgorithm.insertNodes(*left, *right);
				} else {
					if (left) searchAlgorithm.insertNode(*left);
					if (right) searchAlgorithm.insertNode(*right);
				}
			}

		}

		const KdTree& kdTree;
		Real maxRelativeError;
		integer nBruteForce;
		IntervalSequence_ timeIntervalSequence;
	};

	//! Constructs a kd-tree nearest-set.
	/*!
	kdTree:
	The kd-tree to search neighbors in. 
	Either a PointKdTree or a TdTree.

	Optional arguments
	------------------

	maxRelativeError (Real >= 0):
	Maximum allowed relative error in the distance of the 
	result point to the true nearest neighbor. Allowing error
	increases performance. Use 0 for exact matches. 
	Default: 0

	nBruteForce (integer >= 0):
	The number of points under which to start a brute-force
	search in a node. Leaf nodes will always be searched.
	Default: 16

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
	*/
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

		integer nBruteForce = PASTEL_ARG_S(nBruteForce, 8);
		ENSURE_OP(nBruteForce, >=, 0);

		auto&& timeIntervalSequence = 
			PASTEL_ARG_C(
				intervalSequence, 
				(Vector<Real, 2>({-(Real)Infinity(), (Real)Infinity()})),
				Point_Concept);

		auto timeIntervalSequence_ = evaluate(pointAsVector(timeIntervalSequence));
		using IntervalSequence = decltype(timeIntervalSequence_);

		auto&& searchAlgorithmObject =
			PASTEL_ARG_C(
				searchAlgorithm, 
				DepthFirst_SearchAlgorithm_PointKdTree(),
				Trivial_Concept);
			
		using SearchAlgorithm = RemoveCvRef<decltype(searchAlgorithmObject)>;

		return KdTree_NearestSet<KdTree, IntervalSequence, SearchAlgorithm>(
			kdTree,
			maxRelativeError,
			nBruteForce,
			std::move(timeIntervalSequence_));
	}

}

#endif
