// Description: Orthogonal range count in a point kd-tree
// Documentation: pointkdtree_search_range.txt

#ifndef PASTELGEOMETRY_POINTKDTREE_COUNT_RANGE_H
#define PASTELGEOMETRY_POINTKDTREE_COUNT_RANGE_H

#include "pastel/geometry/pointkdtree/pointkdtree.h"
#include "pastel/geometry/shape/alignedbox.h"
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

	//! Counts the number of points contained in an aligned box.
	/*!
	Preconditions:
	bucketSize > 0

	kdTree:
	The kd-tree to count the points in.

	range:
	An open aligned box for which to count the number
	of contained points.

	bucketSize:
	The number of points in a subtree of 'kdTree', under which to 
	perform brute-force searching. A number greater than 1 usually
	gives better performance.

	Returns:
	The number of points contained in the 'range'.
	*/
	template <
		typename Settings, template <typename> class Customization,
		typename Locator = typename Settings::Locator,
		typename Real = typename Locator::Real,
		int N = Locator::N>
	integer countRange(
		const PointKdTree<Settings, Customization>& kdTree,
		const NoDeduction<AlignedBox<Real, N>>& range,
		integer bucketSize = 8)
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
