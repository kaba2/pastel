#ifndef PASTELGEOMETRY_POINTKDTREE_COUNT_RANGE_HPP
#define PASTELGEOMETRY_POINTKDTREE_COUNT_RANGE_HPP

#include "pastel/geometry/pointkdtree/pointkdtree_count_range.h"

#include "pastel/geometry/pointkdtree/pointkdtree_search_range_algorithm.h"

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

	template <
		typename Settings, template <typename> class Customization,
		typename Locator,
		typename Real,
		int N>
	integer countRange(
		const PointKdTree<Settings, Customization>& kdTree,
		const NoDeduction<AlignedBox<Real, N>>& range,
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
