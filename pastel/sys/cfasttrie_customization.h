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

		protected:
			// The customization functions should be protected
			// so that they can only be called by the RedBlackTree
			// implementation.

			using Fwd = RedBlackTree_Fwd<Settings>;

			PASTEL_FWD(Iterator);
			PASTEL_FWD(ConstIterator);
			PASTEL_FWD(Propagation_Class);

			Link_RedBlackTree_Customization() {}

			void onConstruction() {}

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
				integer complexity = infinity<integer>();
				if (!element.data().equalToChain())
				{
					complexity = 
						element.data().complexity();
				}

				propagation.minComplexity = std::min(std::min(
					element.left().propagation().minComplexity, complexity), 
					element.right().propagation().minComplexity);
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
