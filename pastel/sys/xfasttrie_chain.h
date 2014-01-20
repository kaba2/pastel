// Description: X-fast trie node

#ifndef PASTELSYS_XFASTTRIE_NODE_H
#define PASTELSYS_XFASTTRIE_NODE_H

#include "pastel/sys/xfasttrie.h"

#include <unordered_map>
#include <array>
#include <memory>

namespace Pastel
{

	namespace XFastTrie_
	{

		template <
			typename Key,
			typename Iterator>
		class Chain
		{
		public:
			Chain()
			: height_(0)
			, split_(0)
			, element_()
			{
			}

			integer& height()
			{
				return height_;
			}

			Key& split()
			{
				return split_;
			}

			Iterator& element()
			{
				return element_;
			}

		private:
			//! The height of the chain.
			/*!
			A chain is a chain of nodes where the
			chain always follows either the 0-children,
			or the 1-children. The chain spans the
			heights [0, height_].
			*/
			integer height_;

			//! The split-node markers.
			/*!
			The i:th bit is 1 if and only if
			the node at height i is a split 
			node.
			*/
			Key split_;

			//! The key stored in this chain.
			/*!
			The stored key is close to the bottom
			node of the chain.
			*/
			Iterator element_;
		};

	}

}

#endif
