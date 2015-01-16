#ifndef PASTELSYS_REDBLACKTREE_COPY_HPP
#define PASTELSYS_REDBLACKTREE_COPY_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::copyConstruct(
		Node* parent, const RedBlackTree& that, Node* thatNode)
		-> Node*
	{
		if (thatNode->isSentinel())
		{
			return bottomNode();
		}

		integer rollBackIndex = 0;
		Node* node = 0;
		try
		{
			node = allocateNode(
				((Key_Node*)thatNode)->key(), 
				((Key_Node*)thatNode)->data());
			node->parent() = parent;
			node->setRed(thatNode->red());
			node->setSize(thatNode->size());
			++rollBackIndex;
			
			// Copy construct the left child.
			node->left() = copyConstruct(node, that, thatNode->left());
			++rollBackIndex;

			// Copy construct the right child.
			node->right() = copyConstruct(node, that, thatNode->right());
			++rollBackIndex;

			if (thatNode == that.rootNode())
			{
				rootNode() = node;
			}

			if (thatNode == that.minNode())
			{
				minNode() = node;
				minNode()->left() = endNode();
			}
			if (thatNode == that.maxNode())
			{
				maxNode() = node;
				maxNode()->right() = endNode();
			}
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 3:
				clear(node->right());
				// Fall-through.
			case 2:
				clear(node->left());
				// Fall-through.
			case 1:
				deallocateNode((Key_Node*)node);
				break;
			};

			throw;
		}

		return node;
	}

}

#endif
