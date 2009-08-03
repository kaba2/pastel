#ifndef PASTEL_POINTKDTREE_REFINE_HPP
#define PASTEL_POINTKDTREE_REFINE_HPP

#include "pastel/geometry/pointkdtree_refine.h"

#include "pastel/math/largest_eigenvector.h"

namespace Pastel
{

	class Midpoint_SplitRule
	{
	public:
		template <
			int N, typename Real,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const PointKdTree<Real, N, ObjectPolicy>& tree,
			const typename PointKdTree<Real, N, ObjectPolicy>::Cursor& cursor,
			const Point<Real, N>& minBound,
			const Point<Real, N>& maxBound,
			Vector<Real, N>* splitDirection) const
		{
			const integer dimension = tree.dimension();

			// Split along the longest dimension.

			const integer splitAxis = maxIndex(maxBound - minBound);
			const Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			if (splitDirection)
			{
				*splitDirection = unitAxis<Real, N>(dimension, splitAxis);
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

	class MaxVariance_SplitRule
	{
	public:
		template <
			int N, typename Real,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const PointKdTree<Real, N, ObjectPolicy>& tree,
			const typename PointKdTree<Real, N, ObjectPolicy>::Cursor& cursor,
			const Point<Real, N>& minBound,
			const Point<Real, N>& maxBound,
			Vector<Real, N>* splitDirection) const
		{
			typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator
				Iterator;

			const integer dimension = tree.dimension();
			const ObjectPolicy& objectPolicy = tree.objectPolicy();

			std::vector<Point<Real, N> > pointSet;

			Iterator iter = cursor.begin();
			const Iterator iterEnd = cursor.end();
			while(iter != iterEnd)
			{
				pointSet.push_back(objectPolicy.point(*iter));
				++iter;
			}

			const Vector<Real, N> boundExtent = maxBound - minBound;
			const integer shortestSide = minIndex(boundExtent);
			const integer longestSide = maxIndex(boundExtent);

			integer splitAxis = 0;
			Real splitPosition = 0;

			if (boundExtent[longestSide] >= boundExtent[shortestSide] * 3)
			{
				// The node is getting too thin,
				// force sliding midpoint split.
				SlidingMidpoint_SplitRule slidingMidpointRule;
				return slidingMidpointRule(minBound, maxBound,
					objectPolicy, objectBegin, objectEnd, 
					splitDirection);
			}
			else
			{
				const Vector<Real, N> maxVariance = 
					largestEigenVector(pointSet);

				const Point<Real, N> meanPoint = 
					//mean(pointSet);
				linear(minBound, maxBound, 0.5);

				splitAxis = nearestMainAxis(maxVariance);
				splitPosition = meanPoint[splitAxis];

				if (splitDirection)
				{
					splitPosition = dot(asVector(meanPoint), maxVariance);
					*splitDirection = maxVariance;
				}
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

	class SlidingMaxVariance_SplitRule
	{
	public:
		template <
			int N, typename Real,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const PointKdTree<Real, N, ObjectPolicy>& tree,
			const typename PointKdTree<Real, N, ObjectPolicy>::Cursor& cursor,
			const Point<Real, N>& minBound,
			const Point<Real, N>& maxBound,
			Vector<Real, N>* splitDirection) const
		{
			typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator
				ConstObjectIterator;

			const integer dimension = tree.dimension();
			const ObjectPolicy& objectPolicy = tree.objectPolicy();

			std::vector<Point<Real, N> > pointSet;

			{
				ConstObjectIterator iter = cursor.begin();
				const ConstObjectIterator iterEnd = cursor.end();
				while(iter != iterEnd)
				{
					pointSet.push_back(objectPolicy.point(*iter));
					++iter;
				}
			}

			const Vector<Real, N> boundExtent = maxBound - minBound;
			const integer shortestSide = minIndex(boundExtent);
			const integer longestSide = maxIndex(boundExtent);

			integer splitAxis = 0;
			Real splitPosition = 0;

			if (boundExtent[longestSide] >= boundExtent[shortestSide] * 2)
			{
				// The node is getting too thin,
				// force sliding midpoint split.
				SlidingMidpoint_SplitRule slidingMidpointRule;
				return slidingMidpointRule(minBound, maxBound,
					objectPolicy, objectBegin, objectEnd, 
					splitDirection);
			}
			else
			{
				const Vector<Real, N> maxVariance = 
					largestEigenVector(pointSet);

				const Point<Real, N> meanPoint = 
					mean(pointSet);
					//linear(minBound, maxBound, 0.5);

				splitAxis = nearestMainAxis(maxVariance);
				splitPosition = meanPoint[splitAxis];

				AlignedBox<Real, 1> objectBound;

				if (splitDirection)
				{
					splitPosition = dot(asVector(meanPoint), maxVariance);
					*splitDirection = maxVariance;

					ConstObjectIterator iter = cursor.begin();
					const ConstObjectIterator iterEnd = cursor.end();
					while(iter != iterEnd)
					{
						extendToCover(
							Point<Real, 1>(
							dot(asVector(objectPolicy.point(*iter)), maxVariance)),
							objectBound);

						++iter;
					}
				}
				else
				{
					ConstObjectIterator iter = cursor.begin();
					const ConstObjectIterator iterEnd = cursor.end();
					while(iter != iterEnd)
					{
						extendToCover(
							Point<Real, 1>(
							objectPolicy.point(*iter, splitAxis)),
							objectBound);

						++iter;
					}
				}

				if (splitPosition < objectBound.min()[0])
				{
					splitPosition = objectBound.min()[0];
				}
				if (splitPosition > objectBound.max()[0])
				{
					splitPosition = objectBound.max()[0];
				}
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

	class SlidingMidpoint_SplitRule
	{
	public:
		template <
			int N, typename Real,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const PointKdTree<Real, N, ObjectPolicy>& tree,
			const typename PointKdTree<Real, N, ObjectPolicy>::Cursor& cursor,
			const Point<Real, N>& minBound,
			const Point<Real, N>& maxBound,
			Vector<Real, N>* splitDirection) const
		{
			typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator 
				ConstObjectIterator;

			const integer dimension = tree.dimension();
			const ObjectPolicy& objectPolicy = tree.objectPolicy();

			// Split along the longest dimension.

			const integer splitAxis = maxIndex(maxBound - minBound);
			Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			// Sliding midpoint

			if (cursor.objects() > 0)
			{
				Real leftMax = -infinity<Real>();
				Real rightMin = infinity<Real>();
				integer leftCount = 0;
				integer rightCount = 0;

				ConstObjectIterator iter = cursor.begin();
				const ConstObjectIterator iterEnd = cursor.end();
				while(iter != iterEnd)
				{
					const Real position = objectPolicy.point(iter->object(), splitAxis);
					if (position < splitPosition)
					{
						if (position > leftMax)
						{
							leftMax = position;
						}
						++leftCount;
					}
					else
					{
						if (position < rightMin)
						{
							rightMin = position;
						}
						++rightCount;
					}

					++iter;
				}

				if (leftCount == 0)
				{
					splitPosition = rightMin;
				}
				else if (rightCount == 0)
				{
					splitPosition = leftMax;
				}
			}

			if (splitDirection)
			{
				*splitDirection = unitAxis<Real, N>(dimension, splitAxis);
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

	class SlidingMidpoint2_SplitRule
	{
	public:
		template <
			int N, typename Real,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const PointKdTree<Real, N, ObjectPolicy>& tree,
			const typename PointKdTree<Real, N, ObjectPolicy>::Cursor& cursor,
			const Point<Real, N>& minBound,
			const Point<Real, N>& maxBound,
			Vector<Real, N>* splitDirection) const
		{
			typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator 
				ConstObjectIterator;

			const integer dimension = tree.dimension();
			const ObjectPolicy& objectPolicy = tree.objectPolicy();

			// Split along the longest dimension.

			const integer splitAxis = maxIndex(maxBound - minBound);
			Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			// Sliding midpoint

			if (cursor.objects() > 0)
			{
				Real leftMax = -infinity<Real>();
				Real rightMin = infinity<Real>();
				integer leftCount = 0;
				integer rightCount = 0;

				ConstObjectIterator iter = cursor.begin();
				const ConstObjectIterator iterEnd = cursor.end();
				while(iter != iterEnd)
				{
					const Real position = objectPolicy.point(iter->object(), splitAxis);
					if (position < splitPosition)
					{
						if (position > leftMax)
						{
							leftMax = position;
						}
						++leftCount;
					}
					else
					{
						if (position < rightMin)
						{
							rightMin = position;
						}
						++rightCount;
					}

					++iter;
				}

				if (leftCount > 0)
				{
					if (rightCount > 0)
					{
						if (leftCount < rightCount)
						{
							splitPosition = rightMin;
						}
						else
						{
							splitPosition = leftMax;
						}
						/*
						splitPosition = (splitPosition - leftMax) < 
							(rightMin - splitPosition) ?
							//leftMax : rightMin;
							rightMin : leftMax;
						*/
					}
					else
					{
						splitPosition = leftMax;
					}
				}
				else
				{
					ASSERT(rightCount > 0);
					splitPosition = rightMin;
				}
			}

			if (splitDirection)
			{
				*splitDirection = unitAxis<Real, N>(dimension, splitAxis);
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
