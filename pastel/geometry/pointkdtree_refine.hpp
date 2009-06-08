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
			const Point<N, Real>& minBound,
			const Point<N, Real>& maxBound,
			const ObjectPolicy& objectPolicy,
			const typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectBegin,
			const typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectEnd,
			Vector<N, Real>* splitDirection) const
		{
			// Split along the longest dimension.

			const integer splitAxis = maxIndex(maxBound - minBound);
			const Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			if (splitDirection)
			{
				*splitDirection = unitAxis<N, Real>(minBound.dimension(), splitAxis);
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
			const Point<N, Real>& minBound,
			const Point<N, Real>& maxBound,
			const ObjectPolicy& objectPolicy,
			const typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectBegin,
			const typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectEnd,
			Vector<N, Real>* splitDirection) const
		{
			typedef typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator
				Iterator;

			std::vector<Point<N, Real> > pointSet;

			Iterator iter = objectBegin;
			const Iterator iterEnd = objectEnd;
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
			const Point<N, Real>& minBound,
			const Point<N, Real>& maxBound,
			const ObjectPolicy& objectPolicy,
			const typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectBegin,
			const typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectEnd,
			Vector<N, Real>* splitDirection) const
		{
			typedef typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator
				ConstObjectIterator;

			std::vector<Point<N, Real> > pointSet;

			{
				ConstObjectIterator iter = objectBegin;
				const ConstObjectIterator iterEnd = objectEnd;
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
					//mean(pointSet);
					linear(minBound, maxBound, 0.5);

				splitAxis = nearestMainAxis(maxVariance);
				splitPosition = meanPoint[splitAxis];

				AlignedBox<1, Real> objectBound;

				if (splitDirection)
				{
					splitPosition = dot(asVector(meanPoint), maxVariance);
					*splitDirection = maxVariance;

					ConstObjectIterator iter = objectBegin;
					ConstObjectIterator iterEnd = objectEnd;
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
					ConstObjectIterator iter = objectBegin;
					ConstObjectIterator iterEnd = objectEnd;
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

	class SlidingMinSpread_SplitRule
	{
	public:
		template <
			int N, typename Real,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const Point<N, Real>& minBound,
			const Point<N, Real>& maxBound,
			const ObjectPolicy& objectPolicy,
			const typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectBegin,
			const typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectEnd,
			Vector<N, Real>* splitDirection) const
		{
			typedef typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator 
				ConstObjectIterator;

			const integer dimension = minBound.dimension();

			AlignedBox<N, Real> objectBound(dimension);

			ConstObjectIterator iter = objectBegin;
			ConstObjectIterator iterEnd = objectEnd;
			while(iter != iterEnd)
			{
				extendToCover(
					objectPolicy.point(*iter),
					objectBound);

				++iter;
			}

			const Vector<N, Real> boundExtent = maxBound - minBound;

			const Vector<N, Real> relativeSpread(
				objectBound.extent() / boundExtent);

			// Split along the dimension which has minimal
			// relative spread.

			integer splitAxis = minIndex(relativeSpread);
			if (relativeSpread[splitAxis] > 0.5)
			{
				splitAxis = maxIndex(boundExtent);
			}

			Real splitPosition = 
				linear(minBound[splitAxis], maxBound[splitAxis], 0.5);

			AlignedBox<1, Real> leftBound;
			AlignedBox<1, Real> rightBound;

			{
				ConstObjectIterator iter = objectBegin;
				ConstObjectIterator iterEnd = objectEnd;
				while(iter != iterEnd)
				{
					const Real position = objectPolicy.point(*iter, splitAxis);
					if (position < splitPosition)
					{
						extendToCover(Point<1, Real>(position), leftBound);
					}
					else
					{
						extendToCover(Point<1, Real>(position), rightBound);
					}

					++iter;
				}
			}

			splitPosition = (splitPosition - leftBound.max()[0]) < 
				(rightBound.min()[0] - splitPosition) ?
				leftBound.max()[0] : rightBound.min()[0];

			if (splitDirection)
			{
				*splitDirection = unitAxis<N, Real>(dimension, splitAxis);
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
			const Point<N, Real>& minBound,
			const Point<N, Real>& maxBound,
			const ObjectPolicy& objectPolicy,
			const typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectBegin,
			const typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectEnd,
			Vector<N, Real>* splitDirection) const
		{
			typedef typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator 
				ConstObjectIterator;

			const integer dimension = minBound.dimension();

			// Split along the longest dimension.

			const integer splitAxis = maxIndex(maxBound - minBound);
			Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			// Sliding midpoint

			if (objectBegin != objectEnd)
			{
				Real leftMax = -infinity<Real>();
				Real rightMin = infinity<Real>();
				integer leftCount = 0;
				integer rightCount = 0;

				ConstObjectIterator iter = objectBegin;
				ConstObjectIterator iterEnd = objectEnd;
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

	class SlidingMidpoint_SplitRule2
	{
	public:
		template <
			int N, typename Real,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const Point<N, Real>& minBound,
			const Point<N, Real>& maxBound,
			const ObjectPolicy& objectPolicy,
			const typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectBegin,
			const typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectEnd,
			Vector<N, Real>* splitDirection) const
		{
			typedef typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator 
				ConstObjectIterator;

			// Find object spread.

			const integer dimension = bound.dimension();

			AlignedBox<N, Real> objectBound(dimension);

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

			const Vector<N, Real> extent = maxBound - minBound;

			const integer maxExtentAxis = maxIndex(extent);
			const Real maxExtent = extent[maxExtentAxis];

			integer maxLegalSpreadAxis = 0;
			Real maxLegalSpread = 0;

			const Vector<N, Real> spread = objectBound.extent();

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

			const integer splitAxis = maxLegalSpreadAxis;

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

			if (splitDirection)
			{
				*splitDirection = unitAxis<N, Real>(minBound.dimension(), splitAxis);
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
