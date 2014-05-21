#ifndef PASTELSYS_REDBLACKTREE_QUANTILE_HPP
#define PASTELSYS_REDBLACKTREE_QUANTILE_HPP

#include "pastel/sys/redblacktree_quantile.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	typename RedBlackTree<Settings, Customization>::ConstIterator
	quantile(
		const RedBlackTree<Settings, Customization>& tree,
		real alpha)
	{
		/*
		Let n = tree.size(), and

		f : ZZ --> RR: f(i) = (i + 0.5) / n.

		Then

		f(i) = alpha
		<=>
		f(i) n = alpha n
		<=>
		i + 0.5 = alpha n
		<=>
		i = alpha n - 0.5

		Since i is an integer, this equation may not have
		a solution. Therefore, we use the closest integer
		instead:

		i = round(alpha n - 0.5) = floor(alpha n)
		*/

		integer searchIndex = std::floor(alpha * tree.size());
		if (searchIndex <= 0)
		{
			return tree.cbegin();
		}
		if (searchIndex >= tree.size() - 1)
		{
			return tree.clast();
		}

		using ConstIterator = 
			typename RedBlackTree<Settings, Customization>::ConstIterator;

		ConstIterator node = tree.croot();

		integer i = node.left().size();
		while (i != searchIndex)
		{
			ASSERT(!node.isSentinel());
		
			bool right = i < searchIndex;
			node = node.child(right);

			if (right)
			{
				i += node.left().size() + 1;
			}
			else
			{
				i -= node.right().size() + 1;
			}
		}

		return node;
	}

}

#endif
