// Description: Quantile of elements in a red-black tree

#ifndef PASTELSYS_REDBLACKTREE_QUANTILE_H
#define PASTELSYS_REDBLACKTREE_QUANTILE_H

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	//! Returns a quantile of the elements.
	/*!
	Time complexity:
	O(log(tree.size())), if the alpha-quantile is not cbegin() or clast(),
	O(1), otherwise

	returns:
	An element with the index closest to floor(alpha * tree.size()). This 
	corresponds to assigning an element with an index i the quantile 
	f(i) = (i + 0.5) / tree.size(), and then returning the element with
	the closest quantile. For example, if the elements are [a, b, c, d, e],
	then (-inf, 0.2) ==> a, [0.2, 0.4) ==> b, [0.4, 0.6) ==> c, [0.6, 0.8) ==> d,
	[0.8, inf) ==> e.
	*/
	template <typename Settings, template <typename> class Customization>
	typename RedBlackTree<Settings, Customization>::ConstIterator
	quantile(
		const RedBlackTree<Settings, Customization>& tree,
		real alpha);

}

#include "pastel/sys/redblacktree/redblacktree_quantile.hpp"

#endif
