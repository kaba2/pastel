#ifndef PASTELSYS_REDBLACKTREE_LINK_HPP
#define PASTELSYS_REDBLACKTREE_LINK_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::linkBefore(
		RedBlackTree& that)
	{
		ENSURE(!linked());

		Sentinel_Node* thatEnd = (Sentinel_Node*)that.endNode();
		Sentinel_Node* thisEnd = (Sentinel_Node*)endNode();

		Sentinel_Node* thatPrev = thatEnd->prev();
		
		thatPrev->next() = thisEnd;
		thisEnd->prev() = thatPrev;
		thisEnd->next() = thatEnd;
		thatEnd->prev() = thisEnd;
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::removeLink()
	{
		Sentinel_Node* thisEnd = (Sentinel_Node*)endNode();
		Sentinel_Node* prev = thisEnd->prev();
		Sentinel_Node* next = thisEnd->next();
		prev->next() = next;
		next->prev() = prev;
		thisEnd->next() = thisEnd;
		thisEnd->prev() = thisEnd;
	}

	template <typename Settings, template <typename> class Customization>
	bool RedBlackTree<Settings, Customization>::linked() const
	{
		Sentinel_Node* thisEnd = (Sentinel_Node*)endNode();
		return thisEnd->next() != thisEnd;
	}

}

#endif
