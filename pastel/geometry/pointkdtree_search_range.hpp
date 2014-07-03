#ifndef PASTELGEOMETRY_POINTKDTREE_SEARCH_RANGE_HPP
#define PASTELGEOMETRY_POINTKDTREE_SEARCH_RANGE_HPP

#include "pastel/geometry/pointkdtree_search_range.h"

#include "pastel/geometry/pointkdtree_search_range_algorithm.h"

namespace Pastel
{

	namespace PointKdTree_Search_Range_
	{

		template <typename Output>
		class Search_Output
		{
		public:
			explicit Search_Output(
				Output& output)
				: output_(output)
			{
			}

			template <typename Iterator>
			void report(
				const Iterator& iter) const
			{
				output_(iter);
			}

			template <typename Iterator>
			void report(
				const Iterator& begin,
				const Iterator& end,
				integer count) const
			{
				Iterator i = begin;
				while (i != end)
				{
					output(i);
					++i;
				}
			}

		private:
			Output& output_;
		};

	}

	template <typename Real, int N, typename PointPolicy, 
		typename Point_ConstIterator_Output>
	void searchRange(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const AlignedBox<Real, N>& range,
		Point_ConstIterator_Output report)
	{
		PointKdTree_Search_Range_::Search_Output<Point_ConstIterator_Output>
			output(report);

		integer bucketSize = 8;
		searchRangeAlgorithm(kdTree, range, output, bucketSize);
	}

}

#endif
