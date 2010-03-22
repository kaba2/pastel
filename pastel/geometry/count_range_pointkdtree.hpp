#ifndef PASTEL_COUNT_RANGE_POINTKDTREE_HPP
#define PASTEL_COUNT_RANGE_POINTKDTREE_HPP

#include "pastel/geometry/count_range_pointkdtree.h"

#include "pastel/geometry/search_range_algorithm_pointkdtree.h"

namespace Pastel
{

	namespace Detail_CountRange
	{

		class Count_Reporter_SearchRange
		{
		public:
			Count_Reporter_SearchRange(
				integer& count)
				: count_(count)
			{
			}

			template <typename Iterator>
			void report(
				const Iterator& begin,
				const Iterator& end,
				integer count) const
			{
				count_ += count;
			}

		private:
			integer& count_;
		};

	}

	template <typename Real, int N, typename ObjectPolicy>
	integer countRange(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const AlignedBox<Real, N>& range,
		integer bucketSize)
	{
		integer count = 0;

		Detail_CountRange::Count_Reporter_SearchRange
			reporter(count);

		searchRangeAlgorithm(kdTree, range, reporter, bucketSize);

		return count;
	}

}

#endif
