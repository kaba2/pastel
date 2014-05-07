// Description: Red black forest set

#ifndef PASTELSYS_REDBLACKFOREST_SET_H
#define PASTELSYS_REDBLACKFOREST_SET_H

#include "pastel/sys/redblackforest_fwd.h"

namespace Pastel
{

	namespace RedBlackForest_
	{

		template <typename Settings>
		class Set
		{
		public:
			using Fwd = RedBlackForest_Fwd<Settings>;

			PASTEL_FWD(Key);
			PASTEL_FWD(Data_Class);
			PASTEL_FWD(ElementSet);
			PASTEL_FWD(Element_Iterator);
			PASTEL_FWD(Element_ConstIterator);

			Set()
			: elementSet_()
			{
			}

			//! Returns the number of elements in the set.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			integer size() const
			{
				return elementSet_.size();
			}

			//! Returns whether an element exists in the set.
			/*!
			Time complexity: O(log(size() + 2))
			Exception safety: nothrow
			*/
			bool exists(const Key& key) const
			{
				return elementSet_.exists(key);
			}

			//! Searches for an element with a given key.
			/*!
			Time complexity: O(log(size() + 2))
			Exception safety: nothrow
			*/
			Element_ConstIterator find(const Key& key) const
			{
				return elementSet_.find(key);
			}

			//! Returns the smallest element >= key.
			/*!
			Time complexity: O(log(size() + 2))
			Exception safety: nothrow
			*/
			Element_ConstIterator lowerBound(const Key& key) const
			{
				return elementSet_.lowerBound(key);
			}

			//! Returns the smallest element > key.
			/*!
			Time complexity: O(log(size() + 2))
			Exception safety: nothrow
			*/
			Element_ConstIterator upperBound(const Key& key) const
			{
				return elementSet_.upperBound(key);
			}

			//! Removes constness from an iterator.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			Element_Iterator cast(const Element_ConstIterator& element)
			{
				return elementSet_.cast(element);
			}

			PASTEL_ITERATOR_FUNCTIONS_PREFIX(Element_, begin, elementSet_.begin());
			PASTEL_ITERATOR_FUNCTIONS_PREFIX(Element_, end, elementSet_.end());
			PASTEL_ITERATOR_FUNCTIONS_PREFIX(Element_, last, elementSet_.last());

		private:
			std::pair<Element_Iterator, bool> insert(
				const Key& key, 
				const Data_Class& data)
			{
				return elementSet_.insert(key, data);
			}

			Element_ConstIterator erase(const Element_ConstIterator& element)
			{
				return elementSet_.erase(element);
			}

			ElementSet split(const Key& key)
			{
				return elementSet_.split(key);
			}

			ElementSet join(
				ElementSet& elementSet)
			{
				return elementSet_.join(elementSet);
			}

			template <typename, template <typename> class>
			friend class RedBlackForest;

			ElementSet elementSet_;
		};

	}		

}

#endif

