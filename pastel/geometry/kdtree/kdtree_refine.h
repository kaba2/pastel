// Description: Split rules for KdTree
// Documentation: kdtree.txt

#ifndef PASTELGEOMETRY_KDTREE_REFINE_H
#define PASTELGEOMETRY_KDTREE_REFINE_H

#include "pastel/geometry/kdtree/kdtree.h"

namespace Pastel
{

	//! Refines a kd-tree using the midpoint rule.
	/*!
	Preconditions:
	maxDepth >= 0
	maxObjects >= 1

	The midpoint rule splits a node
	from the middle along the axis on which the node
	has greatest extent.

	This refinement is naive and should not
	be used for anything. However, it is provided for 
	checking and performance comparison purposes.
	*/
	class Fair_SplitRule_KdTree
	{
	public:
		template <
			typename Real, int N, 
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const Vector<Real, N>& minBound,
			const Vector<Real, N>& maxBound,
			const ObjectPolicy& objectPolicy,
			const typename KdTree<Real, N, ObjectPolicy>::ConstObjectIterator& objectBegin,
			const typename KdTree<Real, N, ObjectPolicy>::ConstObjectIterator& objectEnd) const
		{
			// Split along the longest dimension.

			integer splitAxis = maxIndex(maxBound - minBound);
			Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			return std::make_pair(splitPosition, splitAxis);
		}
	};

	//! Refines a kd-tree using the sliding midpoint rule.
	/*!
	Preconditions:
	maxDepth >= 0
	maxObjects >= 1

	The sliding midpoint rule first tries to split a node
	from the middle along the axis on which the node
	has greatest extent. If all points are on the
	same side of the split plane, the split plane
	is slid to compactly bound those points, while still
	keeping them all on the same side.

	The sliding midpoint rule is ideal for searching
	nearest neighbors for points.
	*/
	class SlidingMidpoint_SplitRule_KdTree
	{
	public:
		template <
			typename Real, int N,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const Vector<Real, N>& minBound,
			const Vector<Real, N>& maxBound,
			const ObjectPolicy& objectPolicy,
			const typename KdTree<Real, N, ObjectPolicy>::ConstObjectIterator& objectBegin,
			const typename KdTree<Real, N, ObjectPolicy>::ConstObjectIterator& objectEnd) const
		{
			typedef typename KdTree<Real, N, ObjectPolicy>::ConstObjectIterator 
				ConstObjectIterator;

			// Split along the longest dimension.

			integer splitAxis = maxIndex(maxBound - minBound);
			Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			// Sliding midpoint

			AlignedBox<Real, 1> objectBound;

			ConstObjectIterator iter = objectBegin;
			ConstObjectIterator iterEnd = objectEnd;
			while(iter != iterEnd)
			{

				const Tuple<Real, 2> objectRange = objectPolicy.bound(*iter, splitAxis);

				objectBound = boundingAlignedBox(objectBound,
					AlignedBox<Real, 1>(
					Vector<Real, 1>(objectRange[0]),
					Vector<Real, 1>(objectRange[1])));

				++iter;
			}

			if (splitPosition < objectBound.min()[0] &&
				objectBound.min()[0] < maxBound[splitAxis])
			{
				splitPosition = objectBound.min()[0];
			}
			if (splitPosition > objectBound.max()[0] &&
				objectBound.max()[0] > minBound[splitAxis])
			{
				splitPosition = objectBound.max()[0];
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

	class SlidingMidpoint_SplitRule_KdTree2
	{
	public:
		template <
			typename Real, int N,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const Vector<Real, N>& minBound,
			const Vector<Real, N>& maxBound,
			const ObjectPolicy& objectPolicy,
			const typename KdTree<Real, N, ObjectPolicy>::ConstObjectIterator& objectBegin,
			const typename KdTree<Real, N, ObjectPolicy>::ConstObjectIterator& objectEnd) const
		{
			typedef typename KdTree<Real, N, ObjectPolicy>::ConstObjectIterator 
				ConstObjectIterator;

			// Find object spread.

			integer dimension = minBound.n();

			AlignedBox<Real, N> objectBound(dimension);

			ConstObjectIterator iter = objectBegin;
			ConstObjectIterator iterEnd = objectEnd;
			while(iter != iterEnd)
			{
				objectBound = boundingAlignedBox(
					objectBound,

					objectPolicy.bound(*iter));

				++iter;
			}

			// Find the longest dimension.

			Vector<Real, N> extent = maxBound - minBound;

			integer maxExtentAxis = maxIndex(extent);
			Real maxExtent = extent[maxExtentAxis];

			integer maxLegalSpreadAxis = 0;
			Real maxLegalSpread = 0;

			Vector<Real, N> spread = objectBound.extent();

			for (integer i = 0;i < dimension;++i)
			{

				if (extent[i] >= 0.8 * maxExtent)
				{
					if (spread[i] >= maxLegalSpread)
					{
						maxLegalSpreadAxis = i;
						maxLegalSpread = spread[i];
					}
				}
			}

			integer splitAxis = maxLegalSpreadAxis;

			Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			if (splitPosition < objectBound.min()[splitAxis])
			{
				splitPosition = objectBound.min()[splitAxis];
			}
			if (splitPosition > objectBound.max()[splitAxis])
			{
				splitPosition = objectBound.max()[splitAxis];
			}

			if (splitPosition < minBound[splitAxis])
			{
				splitPosition = minBound[splitAxis];
			}
			if (splitPosition > maxBound[splitAxis])
			{
				splitPosition = maxBound[splitAxis];
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

	namespace KdTree_
	{

		template <typename Real, int N, typename ObjectPolicy>
		class BoundPoint
		{
		private:
			using Tree = KdTree<Real, N, ObjectPolicy>;
			using ConstObjectIterator = typename Tree::ConstObjectIterator;

		public:
			BoundPoint(
				const Real& position,
				bool start,
				const ConstObjectIterator& object)
				: position_(position)
				, start_(start)
				, object_(object)
			{
			}

			bool operator<(const BoundPoint& that) const
			{
				if (position_ < that.position_)
				{
					return true;
				}

				if (position_ > that.position_)
				{
					return false;
				}

				if (!start_ && that.start_)
				{
					// Ending points before starting points.
					return true;
				}

				if (start_ && !that.start_)
				{
					return false;
				}

				return object_ < that.object_;
			}

			Real position_;
			bool start_;
			ConstObjectIterator object_;
		};

		template <
			typename Real, int N,
			typename ObjectPolicy>
			void refineSurfaceAreaHeuristic(
			const typename KdTree<Real, N, ObjectPolicy>::Cursor& cursor,
			integer depth,
			const AlignedBox<Real, N>& bound,
			integer maxDepth,
			integer maxObjects,
			integer badRefines,
			KdTree<Real, N, ObjectPolicy>& tree)
		{
			using Tree = KdTree<Real, N, ObjectPolicy>;
			using ConstObjectIterator = typename Tree::ConstObjectIterator;

			if (depth >= maxDepth)
			{
				return;
			}

			Real costToTraverse = 1;
			Real costToIntersect = 80;
			Real emptyScale = 0.85;

			if (cursor.leaf())
			{
				// Surface area heuristic

				// This implementation is essentially from the book
				// "Physically based rendering - From theory to implementation"

				// For the surface heuristic area also refer
				// "Heuristic ray shooting algorithms"

				// Start from the axis which
				// has the maximum extent.

				if (cursor.objects() > maxObjects)
				{
					Real nodeArea = area(bound);
					Real invNodeArea = inverse(nodeArea);

					const Real leafCost = cursor.objects() * costToIntersect;

					bool foundSplit = false;
					Real minPosition = 0;
					Real minCost = Infinity();
					integer minAxis = 0;

					integer n = tree.n();

					for (integer axis = 0;axis < n;++axis)
					{
						std::vector<BoundPoint<Real, N, ObjectPolicy> > pointList;

						const integer boundPoints = 2 * cursor.objects();
						pointList.reserve(boundPoints);

						ConstObjectIterator iter = cursor.begin();
						ConstObjectIterator iterEnd = cursor.end();
						while(iter != iterEnd)
						{
							const Tuple<Real, 2> objectBound = tree.objectPolicy().bound(*iter, axis);

							pointList.push_back(BoundPoint<Real, N, ObjectPolicy>(objectBound[0], true, iter));
							pointList.push_back(BoundPoint<Real, N, ObjectPolicy>(objectBound[1], false, iter));

							++iter;
						}

						std::sort(pointList.begin(), pointList.end());

						AlignedBox<Real, N> positiveBound(bound);
						AlignedBox<Real, N> negativeBound(bound);

						integer negativeObjects = 0;
						integer positiveObjects = cursor.objects();
						for (integer i = 0;i < boundPoints;++i)
						{
							if (!pointList[i].start_)
							{
								--positiveObjects;
							}

							dreal position = pointList[i].position_;

							if (position >= bound.min()[axis] &&
								position < bound.max()[axis])
							{
								positiveBound.min()[axis] = position;
								negativeBound.max()[axis] = position;

								// Note that the probabilities
								// of intersecting the negative and
								// the positive child node do not
								// need to sum to one (and they don't).

								Real negativeProbability =
									area(negativeBound) * invNodeArea;
								Real positiveProbability =
									area(positiveBound) * invNodeArea;

								Real scale = 1;
								if (negativeObjects == 0 || positiveObjects == 0)
								{
									scale = emptyScale;
								}

								Real totalCost =
									costToTraverse +

									scale * (negativeProbability * negativeObjects +
									positiveProbability * positiveObjects) * costToIntersect;

								if (totalCost < minCost)
								{
									minCost = totalCost;
									minPosition = position;
									minAxis = axis;
									foundSplit = true;
								}
							}

							if (pointList[i].start_)
							{
								++negativeObjects;
							}
						}
					}

					if (foundSplit)
					{
						if (minCost > leafCost)
						{
							++badRefines;
							if (badRefines == 3)
							{
								return;
							}
						}

						tree.subdivide(cursor, minPosition, minAxis);
					}
				}
			}

			// A leaf node might have been turned
			// into an intermediate node and thus this can't
			// be an else branch.
			if (!cursor.leaf())
			{
				Real splitPosition = cursor.splitPosition();
				integer splitAxis = cursor.splitAxis();

				AlignedBox<Real, N> negativeBound(bound);
				negativeBound.max()[splitAxis] = splitPosition;

				refineSurfaceAreaHeuristic(cursor.negative(), depth + 1,
					negativeBound, maxDepth, maxObjects, badRefines, tree);

				AlignedBox<Real, N> positiveBound(bound);
				positiveBound.min()[splitAxis] = splitPosition;

				refineSurfaceAreaHeuristic(cursor.positive(), depth + 1,
					positiveBound, maxDepth, maxObjects, badRefines, tree);
			}
		}

	}

	//! Refines a kd-tree using the surface area heuristic.
	/*!
	Preconditions:
	maxDepth >= 0
	maxObjects >= 1

	The surface area heuristic tries to minimize
	the cost of traversing the kd-tree with a ray.
	*/
	template <
		typename Real,
		int N,
		typename ObjectPolicy>
		void refineSurfaceAreaHeuristic(
		integer maxDepth,
		integer maxObjects,

		KdTree<Real, N, ObjectPolicy>& tree)
	{
		ENSURE_OP(maxDepth, >=, 0);
		ENSURE_OP(maxObjects, >=, 1);

		KdTree_::refineSurfaceAreaHeuristic(
			tree.root(), 0, tree.bound(), maxDepth, maxObjects, 0, tree);
	}

}

#endif

