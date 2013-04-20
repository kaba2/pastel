#ifndef PASTELGEOMETRY_COUNT_RANGE_POINTKDTREE_HPP
#define PASTELGEOMETRY_COUNT_RANGE_POINTKDTREE_HPP

#include "pastel/geometry/count_range_pointkdtree.h"

#include "pastel/geometry/search_range_algorithm_pointkdtree.h"

namespace Pastel
{

	namespace CountRange_
	{

		class Count_Output_SearchRange
		{
		public:
			Count_Output_SearchRange(
				integer& count)
				: count_(count)
			{
			}

			template <typename Iterator>
			void report(
				const Iterator& iter) const
			{
				++count_;
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

	template <typename Real, int N, typename PointPolicy>
	integer countRange(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const AlignedBox<Real, N>& range,
		integer bucketSize)
	{
		ENSURE_OP(bucketSize, >, 0);

		integer count = 0;

		CountRange_::Count_Output_SearchRange
			reporter(count);

		searchRangeAlgorithm(kdTree, range, reporter, bucketSize);

		return count;
	}

}

#endif
