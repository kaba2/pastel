// Description: Range search in a range tree
// Documentation: rangetree_search_range.txt

#ifndef PASTELGEOMETRY_RANGETREE_SEARCH_RANGE_H
#define PASTELGEOMETRY_RANGETREE_SEARCH_RANGE_H

#include "pastel/geometry/rangetree/rangetree.h"
#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/sys/output/null_output.h"

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

	O(tree.orders()), if the the searched multi-interval 
	is an empty set.

	If the output is Null_Output, then the 
	'k' term disappears. That is, the range search
	degenerates to a range count.

	returns:
	The number of points in the range.
	*/
	template <
		typename Settings,
		template <typename> class Customization,
		typename Point_ConstIterator_Output = Null_Output>
	integer searchRange(
		const RangeTree<Settings, Customization>& tree,
		const typename Settings::Point& min,
		const typename Settings::Point& max,
		const Point_ConstIterator_Output& output = Point_ConstIterator_Output());

}

#include "pastel/geometry/rangetree/rangetree_search_range.hpp"

#endif
