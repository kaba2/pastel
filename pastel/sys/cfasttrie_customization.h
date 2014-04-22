#ifndef PASTELSYS_CFASTTRIE_CUSTOMIZATION_H
#define PASTELSYS_CFASTTRIE_CUSTOMIZATION_H

#include "pastel/sys/redblacktree_fwd.h"

namespace Pastel
{

	namespace CFastTrie_
	{

		template <typename Settings>
		class Link_RedBlackTree_Customization
		{
		public:
			using Tree = RedBlackTree<Settings, CFastTrie_::Link_RedBlackTree_Customization>;

			//! Links the tree with another tree.
			/*!
			Preconditions:
			!linked()

			Time complexity: O(1)
			Exception safety: nothrow

			Each red-black tree is part of exactly one doubly-linked
			loop of trees. In the beginning each tree is the
			only member in its loop. The tree-links are available
			as the next() and prev() functions in the
			iterator, provided that the iterator is in an
			end-node.
			*/
			void linkBefore(Tree& that)
			{
				ENSURE(!linked());

				Iterator thatEnd = that.end();
				Iterator thisEnd = self().end();

				Tree* thatPrev = thatEnd.sentinelData().prev;
				
				thatPrev->end().sentinelData().next = &self();
				thisEnd.sentinelData().prev = thatPrev;
				thisEnd.sentinelData().next = &that;
				thatEnd.sentinelData().prev = &self();
			}

			//! Removes the tree from a link-loop.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			void removeLink()
			{
				Iterator thisEnd = self().end();
				Tree* prev = thisEnd.sentinelData().prev;
				Tree* next = thisEnd.sentinelData().next;
				prev->end().sentinelData().next = next;
				next->end().sentinelData().prev = prev;
				thisEnd.sentinelData().next = &self();
				thisEnd.sentinelData().prev = &self();
			}

			//! Returns whether the tree is part of some link-loop.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			bool linked() const
			{
				return self().end().sentinelData().next != &self();
			}

		protected:
			// The customization functions should be protected
			// so that they can only be called by the RedBlackTree
			// implementation.

			using Fwd = RedBlackTree_Fwd<Settings>;

			PASTEL_FWD(Iterator);
			PASTEL_FWD(ConstIterator);
			PASTEL_FWD(Propagation_Class);

			Link_RedBlackTree_Customization() {}

			void onConstruction()
			{
				self().end().sentinelData().next = &self();
				self().end().sentinelData().prev = &self();
			}

			void swap(Link_RedBlackTree_Customization& that) {}
			void onClear() {}
			void onInsert(const Iterator& element) {}
			void onErase(const Iterator& element) {}
			void onSpliceFrom(const Iterator& element) {}
			void onSplice(const Iterator& element) {}
			
			void updatePropagation(
				const Iterator& element,
				Propagation_Class& propagation) 
			{
				propagation = 
					!element.data().equalToChain() |
					element.left().propagation() |
					element.right().propagation();
			}

		private:
			const Tree& self() const
			{
				return (const Tree&)*this;
			}

			Tree& self()
			{
				return (Tree&)*this;
			}

			Link_RedBlackTree_Customization(
				const Link_RedBlackTree_Customization& that) = delete;
			Link_RedBlackTree_Customization(
				Link_RedBlackTree_Customization&& that) = delete;
			Link_RedBlackTree_Customization& operator=(
				Link_RedBlackTree_Customization) = delete;
		};

	}

}

#endif
