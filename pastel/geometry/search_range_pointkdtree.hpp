#ifndef PASTEL_SEARCH_RANGE_POINTKDTREE_HPP
#define PASTEL_SEARCH_RANGE_POINTKDTREE_HPP

#include "pastel/geometry/search_range_pointkdtree.h"

#include "pastel/geometry/search_range_algorithm_pointkdtree.h"

namespace Pastel
{

	namespace Detail_SearchRange
	{

		template <typename OutputIterator>
		class Search_Reporter_SearchRange
		{
		public:
			explicit Search_Reporter_SearchRange(
				OutputIterator& output)
				: output_(output)
			{
			}

			template <typename Iterator>
			void report(
				const Iterator& begin,
				const Iterator& end,
				integer count) const
			{
				output_ = std::copy(begin, end, output_);
			}

		private:
			OutputIterator& output_;
		};

	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename ObjectIterator_OutputIterator>
	void searchRange(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const AlignedBox<Real, N>& range,
		ObjectIterator_OutputIterator result,
		integer bucketSize)
	{
		Detail_SearchRange::Search_Reporter_SearchRange<ObjectIterator_OutputIterator>
			reporter(result);

		searchRangeAlgorithm(kdTree, range, reporter, bucketSize);
	}

}

#endif
