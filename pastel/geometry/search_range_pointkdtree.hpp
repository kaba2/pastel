#ifndef PASTELGEOMETRY_SEARCH_RANGE_POINTKDTREE_HPP
#define PASTELGEOMETRY_SEARCH_RANGE_POINTKDTREE_HPP

#include "pastel/geometry/search_range_pointkdtree.h"

#include "pastel/geometry/search_range_algorithm_pointkdtree.h"

namespace Pastel
{

	namespace SearchRange_
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
		SearchRange_::Search_Reporter_SearchRange<Point_ConstIterator_Iterator>
			reporter(result);

		searchRangeAlgorithm(kdTree, range, reporter, bucketSize);
	}

}

#endif
