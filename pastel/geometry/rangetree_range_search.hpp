#ifndef PASTELGEOMETRY_RANGETREE_RANGE_SEARCH_HPP
#define PASTELGEOMETRY_RANGETREE_RANGE_SEARCH_HPP

#include "pastel/geometry/rangetree_range_search.h"

namespace Pastel
{

	template <
		typename Settings, 
		template <typename> class Customization,
		typename Point_ConstIterator_Output,
		typename X_Less,
		typename Y_Less>
	integer rangeSearch(
		const RangeTree<Settings, Customization>& tree,
		const typename Settings::Point& min,
		const typename Settings::Point& max,
		const X_Less& xLess,
		const Y_Less& yLess,
		const Point_ConstIterator_Output& output)
	{
		using Tree = RangeTree<Settings, Customization>;
		using Node = typename Tree::Node_ConstIterator;

		Node node = tree.root();

		return 0;
	}

}

#endif
