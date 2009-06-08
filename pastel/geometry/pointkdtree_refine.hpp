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
			const PointKdTree<N, Real, ObjectPolicy>& tree,
			const typename PointKdTree<N, Real, ObjectPolicy>::Cursor& cursor,
			const Point<N, Real>& minBound,
			const Point<N, Real>& maxBound,
			Vector<N, Real>* splitDirection) const
		{
			const integer dimension = tree.dimension();

			// Split along the longest dimension.

			const integer splitAxis = maxIndex(maxBound - minBound);
			const Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			if (splitDirection)
			{
				*splitDirection = unitAxis<N, Real>(dimension, splitAxis);
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
			const PointKdTree<N, Real, ObjectPolicy>& tree,
			const typename PointKdTree<N, Real, ObjectPolicy>::Cursor& cursor,
			const Point<N, Real>& minBound,
			const Point<N, Real>& maxBound,
			Vector<N, Real>* splitDirection) const
		{
			typedef typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator
				Iterator;

			const integer dimension = tree.dimension();
			const ObjectPolicy& objectPolicy = tree.objectPolicy();

			std::vector<Point<N, Real> > pointSet;

			Iterator iter = cursor.begin();
			const Iterator iterEnd = cursor.end();
			while(iter != iterEnd)
			{
				pointSet.push_back(objectPolicy.point(*iter));
				++iter;
			}

			const Vector<N, Real> boundExtent = maxBound - minBound;
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
				const Vector<N, Real> maxVariance = 
					largestEigenVector(pointSet);

				const Point<N, Real> meanPoint = 
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
			const PointKdTree<N, Real, ObjectPolicy>& tree,
			const typename PointKdTree<N, Real, ObjectPolicy>::Cursor& cursor,
			const Point<N, Real>& minBound,
			const Point<N, Real>& maxBound,
			Vector<N, Real>* splitDirection) const
		{
			typedef typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator
				ConstObjectIterator;

			const integer dimension = tree.dimension();
			const ObjectPolicy& objectPolicy = tree.objectPolicy();

			std::vector<Point<N, Real> > pointSet;

			{
				ConstObjectIterator iter = cursor.begin();
				const ConstObjectIterator iterEnd = cursor.end();
				while(iter != iterEnd)
				{
					pointSet.push_back(objectPolicy.point(*iter));
					++iter;
				}
			}

			const Vector<N, Real> boundExtent = maxBound - minBound;
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
				const Vector<N, Real> maxVariance = 
					largestEigenVector(pointSet);

				const Point<N, Real> meanPoint = 
					mean(pointSet);
					//linear(minBound, maxBound, 0.5);

				splitAxis = nearestMainAxis(maxVariance);
				splitPosition = meanPoint[splitAxis];

				AlignedBox<1, Real> objectBound;

				if (splitDirection)
				{
					splitPosition = dot(asVector(meanPoint), maxVariance);
					*splitDirection = maxVariance;

					ConstObjectIterator iter = cursor.begin();
					const ConstObjectIterator iterEnd = cursor.end();
					while(iter != iterEnd)
					{
						extendToCover(
							Point<1, Real>(
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
							Point<1, Real>(
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
			const PointKdTree<N, Real, ObjectPolicy>& tree,
			const typename PointKdTree<N, Real, ObjectPolicy>::Cursor& cursor,
			const Point<N, Real>& minBound,
			const Point<N, Real>& maxBound,
			Vector<N, Real>* splitDirection) const
		{
			typedef typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator 
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
					const Real position = objectPolicy.point(*iter, splitAxis);
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
				*splitDirection = unitAxis<N, Real>(dimension, splitAxis);
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
			const PointKdTree<N, Real, ObjectPolicy>& tree,
			const typename PointKdTree<N, Real, ObjectPolicy>::Cursor& cursor,
			const Point<N, Real>& minBound,
			const Point<N, Real>& maxBound,
			Vector<N, Real>* splitDirection) const
		{
			typedef typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator 
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
					const Real position = objectPolicy.point(*iter, splitAxis);
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
				*splitDirection = unitAxis<N, Real>(dimension, splitAxis);
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
