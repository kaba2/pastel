// Description: Range search in a range tree
// Documentation: rangetree.txt

#ifndef PASTELGEOMETRY_RANGETREE_RANGE_SEARCH_H
#define PASTELGEOMETRY_RANGETREE_RANGE_SEARCH_H

#include "pastel/geometry/rangetree.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/sys/null_output.h"

namespace Pastel
{

	//! Range search in a range tree.
	/*!
	Reports the points contained in the closed 
	multi-interval [min, max].

	Time complexity:
	O(k + log(tree.size())^(tree.orders() - 1))
	where
	k is the number of reported points.

	If the output is Null_Output, then the 
	'k' term disappears.

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
