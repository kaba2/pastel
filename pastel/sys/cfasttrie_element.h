// Description: C-fast trie element

#ifndef PASTELSYS_CFASTTRIE_ELEMENT_H
#define PASTELSYS_CFASTTRIE_ELEMENT_H

#include "pastel/sys/cfasttrie_fwd.h"

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	class CFastTrie;
	
	namespace CFastTrie_
	{

		//! C-fast trie element
		/*!
		An element stores the key, the potential user-data, and
		the corresponding chain. We make use of the empty base-class 
		optimization to avoid memory-use if Value = void.
		*/
		template <typename CFastTrie_Settings>
		class Element
			: public CFastTrie_Fwd<CFastTrie_Settings>::Value_Class
		{
		public:
			using Fwd = CFastTrie_Fwd<CFastTrie_Settings>;

			PASTEL_FWD(Fork_Iterator);
			PASTEL_FWD(Bundle_Iterator);

			// Using the name Value_Class here triggers a
			// bug in Visual Studio 2013.
			using Value_Class_ = typename Fwd::Value_Class;
			//PASTEL_FWD(Value_Class);

			Element()
				: Value_Class_()
				, bundle_()
			{
			}

			Element(
				const Bundle_Iterator& bundle,
				bool equalToChain,
				integer complexity,
				const Value_Class_& value)
				: Value_Class_(value)
				, bundle_(bundle)
				, equalToChain_(equalToChain)
				, complexity_(complexity)
			{
			}

			bool equalToChain() const
			{
				return equalToChain_;
			}

			integer complexity() const
			{
				return complexity_;
			}

		private:
			template <
				typename Settings,
				template <typename> class Customization>
			friend class CFastTrie;

			template <typename CFastTrie_Settings>
			friend class Bundle;
				
			//! Returns the bundle referencing this element.
			const Bundle_Iterator& bundle() const
			{
				return bundle_;
			}

			//! The bundle referencing this element.
			/*!
			An element references a bundle if and only
			the bundle references the element. Not every
			element references a bundle.
			*/
			Bundle_Iterator bundle_;

			//! Whether this element is equal to some chain.
			/*!
			The insertion algorithm needs to know whether an
			element is equal to a chain. If so, it needs to 
			find the smallest next (greatest previous) element 
			which is not equal to a chain.
			*/
			bool equalToChain_ = false;

			//! The complexity of the element.
			/*!
			The complexity of an integer is the number of
			01 and 10 pairs in its binary representation.
			For example, c(1001_2) = 3.
			*/
			integer complexity_;
		};

	}

}

#endif
