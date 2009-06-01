#ifndef PASTEL_POINTKDTREE_REFINE_HPP
#define PASTEL_POINTKDTREE_REFINE_HPP

#include "pastel/geometry/pointkdtree_refine.h"

namespace Pastel
{

	class MidpointRule
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
			const typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectEnd) const
		{
			// Split along the longest dimension.

			const integer splitAxis = maxIndex(maxBound - minBound);
			const Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			return std::make_pair(splitPosition, splitAxis);
		}
	};

	class SlidingMidpointRule
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
			const typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectEnd) const
		{
			typedef typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator 
				ConstObjectIterator;

			// Split along the longest dimension.

			const integer splitAxis = maxIndex(maxBound - minBound);
			Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			// Sliding midpoint

			AlignedBox<1, Real> objectBound;

			ConstObjectIterator iter = objectBegin;
			ConstObjectIterator iterEnd = objectEnd;
			while(iter != iterEnd)
			{
				extendToCover(
					Point<1, Real>(objectPolicy.point(*iter, splitAxis)),
					objectBound);

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

	class SlidingMidpointRule2
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
			const typename PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectEnd) const
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

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
