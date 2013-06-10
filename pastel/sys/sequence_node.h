// Description: Sequence node

#ifndef PASTELSYS_SEQUENCE_NODE_H
#define PASTELSYS_SEQUENCE_NODE_H

#include "pastel/sys/sequence.h"
#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	namespace Sequence_
	{

		class Node
		{
		public:
			Node()
			: child_()
			, parent_(0)
			, shortcut_(0)
			{
				child_[0] = 0;
				child_[1] = 0;
			}

			virtual ~Node()
			{
			}

			Node*& child(bool right)
			{
				return child_[right];
			}

			Node* child(bool right) const
			{
				return child_[right];
			}

			Node*& left()
			{
				return child_[0];
			}

			Node* left() const
			{
				return child_[0];
			}

			Node*& right()
			{
				return child_[1];
			}

			Node* right() const
			{
				return child_[1];
			}

			bool isLeaf() const
			{
				return !left() && !right() && parent();
			}

			bool splitsBoth() const
			{
				return left() && right();
			}

			Node*& parent()
			{
				return parent_;
			}

			Node* parent() const
			{
				return parent_;
			}

			Leaf_Node*& shortcut()
			{
				return shortcut_;
			}

			Leaf_Node* shortcut() const
			{
				return shortcut_;
			}

			std::array<Node*, 2> child_;
			Node* parent_;
			Leaf_Node* shortcut_;
		};

		template <typename SkipList>
		class Leaf_Node
		: public Node
		{
		public:
			Leaf_node()
			: Node()
			, nearby_()
			{				
			}
			
			virtual ~Leaf_Node()
			{
			}

			typename SkipList::Iterator nearby;
		};	

	}

}

#endif
