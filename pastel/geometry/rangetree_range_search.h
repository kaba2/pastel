// Description: Range search in a range tree
// Documentation: rangetree.txt

#ifndef PASTELGEOMETRY_RANGETREE_RANGE_SEARCH_H
#define PASTELGEOMETRY_RANGETREE_RANGE_SEARCH_H

#include "pastel/geometry/rangetree.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/sys/null_output.h"

namespace Pastel
{

	//! Orthogonal range search in a range tree.
	/*!
	Time complexity:
	O(log(tree.size())^(tree.orders() - 1))

	returns:
	The number of points in the range.
	*/
	template <
		typename Settings,
		template <typename> class Customization,
		typename Point_ConstIterator_Output = Null_Output>
		integer rangeSearch(
		const RangeTree<Settings, Customization>& tree,
		const typename Settings::Point& min,
		const typename Settings::Point& max,
		const Point_ConstIterator_Output& output = Point_ConstIterator_Output());

}

#include "pastel/geometry/rangetree_range_search.hpp"

#endif
