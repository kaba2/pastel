#ifndef PASTELGEOMETRY_POINTKDTREE_SEARCH_RANGE_HPP
#define PASTELGEOMETRY_POINTKDTREE_SEARCH_RANGE_HPP

#include "pastel/geometry/pointkdtree_search_range.h"

#include "pastel/geometry/pointkdtree_search_range_algorithm.h"

namespace Pastel
{

	namespace SearchRange_
	{

		template <typename OutputIterator>
		class Search_Output_SearchRange
		{
		public:
			explicit Search_Output_SearchRange(
				OutputIterator& output)
				: output_(output)
			{
			}

			template <typename Iterator>
			void report(
				const Iterator& iter) const
			{
				*output_ = iter;
				++output_;
			}

			template <typename Iterator>
			void report(
				const Iterator& begin,
				const Iterator& end,
				integer count) const
			{
				output_ = std::copy(
					countingIterator(begin), 
					countingIterator(end), 
					output_);
			}

		private:
			OutputIterator& output_;
		};

	}

	template <typename Real, int N, typename PointPolicy, 
		typename Point_ConstIterator_Iterator>
	void searchRange(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const AlignedBox<Real, N>& range,
		Point_ConstIterator_Iterator result,
		integer bucketSize)
	{
		SearchRange_::Search_Output_SearchRange<Point_ConstIterator_Iterator>
			reporter(result);

		searchRangeAlgorithm(kdTree, range, reporter, bucketSize);
	}

}

#endif
