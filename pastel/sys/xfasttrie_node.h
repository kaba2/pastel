// Description: X-fast trie node

#ifndef PASTELSYS_XFASTTRIE_NODE_H
#define PASTELSYS_XFASTTRIE_NODE_H

#include "pastel/sys/xfasttrie.h"

#include <unordered_map>

namespace Pastel
{

	namespace XFastTrie_
	{

		template <typename Key, typename Iterator>
		class Node
		{
		public:
			Node()
			: child_()
			, shortcut_(0)
			, element_()
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
				return left() == right();
			}

			bool splitsBoth() const
			{
				return (left() != shortcut()) &&
					(right() != shortcut());
			}

			Node*& shortcut()
			{
				return shortcut_;
			}

			Node* shortcut() const
			{
				return shortcut_;
			}

			std::array<Node*, 2> child_;
			Node* shortcut_;
			Iterator element_;
		};

	}

}

#endif
