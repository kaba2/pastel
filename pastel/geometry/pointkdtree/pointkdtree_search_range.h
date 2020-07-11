// Description: Orthogonal range search in a kd-tree
// Documentation: pointkdtree_search_range.txt

#ifndef PASTELGEOMETRY_POINTKDTREE_SEARCH_RANGE_H
#define PASTELGEOMETRY_POINTKDTREE_SEARCH_RANGE_H

#include "pastel/geometry/pointkdtree/pointkdtree.h"
#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/pointkdtree/pointkdtree_search_range_algorithm.h"

#include "pastel/sys/output/output_concept.h"

#include <vector>

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

	//! Finds all points contained in an aligned box using a kd-tree.
	/*!
	Preconditions:
	kdTree.n() == range.n()
	kdTree.n() <= 32
	*/
	template <
		typename Settings, template <typename> class Customization, 
		typename Point_ConstIterator_Output>
	void searchRange(
		const PointKdTree<Settings, Customization>& kdTree,
		const AlignedBox<typename Settings::Real, Settings::N>& range,
		Point_ConstIterator_Output report)
	{
		PointKdTree_Search_Range_::Search_Output<Point_ConstIterator_Output>
			output(report);

		integer bucketSize = 8;
		searchRangeAlgorithm(kdTree, range, output, bucketSize);
	}

}

#endif
