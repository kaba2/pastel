// Description: C-fast trie element

#ifndef PASTELSYS_CFASTTRIE_ELEMENT_H
#define PASTELSYS_CFASTTRIE_ELEMENT_H

#include "pastel/sys/cfasttrie_fwd.h"

namespace Pastel
{

	template <
		typename Settings,
		typename Customization>
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
			PASTEL_FWD(BundlePtr);

			// Using the name Value_Class here triggers a
			// bug in Visual Studio 2013.
			using Value_Class_ = typename Fwd::Value_Class;
			//PASTEL_FWD(Value_Class);

			Element()
				: Value_Class_()
				, bundle_()
			{
			}

			template <typename... Value>
			Element(
				const BundlePtr& bundle,
				Value&&... value)
				: Value_Class_(std::forward<Value>(value)...)
				, bundle_(bundle)
			{
			}

		private:
			template <
				typename Settings,
				typename Customization>
			friend class CFastTrie;

			//! Returns the bundle referencing this element.
			const BundlePtr& bundle() const
			{
				return bundle_;
			}

			//! The bundle referencing this element.
			/*!
			An element references a bundle if and only
			the bundle references the element. Not every
			element references a bundle.
			*/
			BundlePtr bundle_;

			//! The fork referencing this element.
			Fork_Iterator fork_;
		};

	}

}

#endif
