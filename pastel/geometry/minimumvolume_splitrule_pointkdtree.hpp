#ifndef PASTEL_MINIMUMVOLUME_SPLITRULE_POINTKDTREE_HPP
#define PASTEL_MINIMUMVOLUME_SPLITRULE_POINTKDTREE_HPP

#include "pastel/geometry/minimumvolume_splitrule_pointkdtree.h"
#include "pastel/geometry/pointkdtree.h"

#include "pastel/sys/vector_tools.h"

#include <algorithm>

namespace Pastel
{

	class MinimumVolume_SplitRule_PointKdTree
	{
	public:
		template <
			typename Real, int N,
			typename PointPolicy>
			std::pair<Real, integer> operator()(
			const PointKdTree<Real, N, PointPolicy>& tree,
			const typename PointKdTree<Real, N, PointPolicy>::Cursor& cursor,
			const Vector<Real, N>& minBound,
			const Vector<Real, N>& maxBound,
			integer depth) const
		{
			typedef typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator 
				Point_ConstIterator;

			const PointPolicy& pointPolicy = tree.pointPolicy();

			const integer n = tree.dimension();

			Vector<Real, N> projectedArea(ofDimension(n), 1);
			for (integer i = 0;i < n;++i)
			{
				for (integer j = 0;j < n;++j)
				{
					if (j != i)
					{
						projectedArea[i] *= maxBound[j] - minBound[j];
					}
				}
			}

			Real maxCut = 0;
			integer maxCutIndex = maxIndex(maxBound - minBound);
			Real maxCutPosition = linear(minBound[maxCutIndex], maxBound[maxCutIndex], 0.5);;

			Real maxMedianCut = 0;
			integer maxMedianCutIndex = maxIndex(maxBound - minBound);
			Real maxMedianCutPosition = linear(minBound[maxCutIndex], maxBound[maxCutIndex], 0.5);

			if (!cursor.empty())
			{
				for (integer i = 0;i < tree.dimension();++i)
				{
					// Get the positions of the points along the splitting axis.

					std::vector<Real> positionSet;
					positionSet.reserve(cursor.points());

					Point_ConstIterator iter = cursor.begin();
					const Point_ConstIterator iterEnd = cursor.end();
					while(iter != iterEnd)
					{
						positionSet.push_back(pointPolicy.axis(iter->point(), i));
						++iter;
					}

					std::sort(positionSet.begin(), positionSet.end());

					// Find the maximal gap

					Real maxGap = -infinity<Real>();
					integer maxGapIndex = 0;
					Real maxGapPosition = 0;
					for (integer j = 0;j < positionSet.size() - 1;++j)
					{
						const Real gap = positionSet[j + 1] - positionSet[j];
						if (gap > maxGap)
						{
							maxGap = gap;
							maxGapIndex = j;
							maxGapPosition = linear(positionSet[j], positionSet[j + 1], 0.5);
						}
					}

					const Real boundaryGap = 
						(positionSet.front() - minBound[i]) +
						(maxBound[i] - positionSet.back());

					const Real cut = (maxGap + boundaryGap)/* * projectedArea[i]*/;
					if (cut > maxCut)
					{
						maxCut = cut;
						maxCutIndex = i;
						maxCutPosition = maxGapPosition;
					}

					// Get the median of the points on the splitting axis.
					integer medianIndex = positionSet.size() / 2;
					Real medianGap = 0;
					Real medianGapPosition = positionSet[medianIndex];
					if (positionSet.size() > 1)
					{
						medianGap = positionSet[medianIndex] - positionSet[medianIndex - 1];
						medianGapPosition = linear(positionSet[medianIndex], positionSet[medianIndex - 1], 0.5);
					}

					const Real medianCut = (medianGap + boundaryGap)/* * projectedArea[i]*/;
					if (medianCut > maxMedianCut)
					{
						maxMedianCut = medianCut;
						maxMedianCutIndex = i;
						maxMedianCutPosition = medianGapPosition;
					}
				}
			}

			// We choose the median if the maximum cut is less than 25%, 
			// or the median can provide at least 75% of the maximum cut.

			if (maxCut < 0.25 * (maxBound[maxCutIndex] - minBound[maxCutIndex]) ||
				maxMedianCut >= maxCut * 0.75)
			{
				return std::make_pair(maxMedianCutPosition, maxMedianCutIndex);
			}

			return std::make_pair(maxCutPosition, maxCutIndex);
		}
	};

}

#endif
