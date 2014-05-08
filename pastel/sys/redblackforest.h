// Description: Red-black forest

#ifndef PASTELSYS_REDBLACKFOREST_H
#define PASTELSYS_REDBLACKFOREST_H

#include "pastel/sys/redblackforest_concepts.h"
#include "pastel/sys/redblackforest_fwd.h"
#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings>
	using Empty_RedBlackForest_Customization =
		RedBlackForest_Concepts::Customization<Settings>;

	template <
		typename Settings_, 
		template <typename> class Customization_ = Empty_RedBlackForest_Customization>
	class RedBlackForest
	{
	public:
		using Settings = Settings_;
		using Fwd = RedBlackForest_Fwd<Settings>;

		PASTEL_FWD(ElementSet);
		PASTEL_FWD(Element_Iterator);
		PASTEL_FWD(Element_ConstIterator);

		PASTEL_FWD(SetSet);
		PASTEL_FWD(Set_Iterator);
		PASTEL_FWD(Set_ConstIterator);

		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);
		PASTEL_FWD(Range);
		PASTEL_FWD(ConstRange);

		using Customization = Customization_<Settings>;

		//! Constructs an empty forest.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		RedBlackForest()
		: setSet_()
		{
			// Create the first set.
			insert(setSet_.cend());
			// Create the last set.
			insert(setSet_.cend());
		}

		//! Copy-constructs from another forest.
		/*!
		Time complexity: O(that.sets() + that.size())
		Exception safety: strong
		*/
		RedBlackForest(const RedBlackForest& that)
		: RedBlackForest()
		{
			for (auto&& set : that.csetRange())
			{
				setSet_.emplace(csetEnd(), set);
			}
		}

		//! Move-constructs from another forest.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		RedBlackForest(RedBlackForest&& that)
		: RedBlackForest()
		{
			swap(that);
		}
		
		//! Assigns from another forest.
		/*!
		Time complexity: O(that.sets() + that.size())
		Exception safety: strong
		*/
		RedBlackForest& operator=(RedBlackForest that)
		{
			swap(that);
			return *this;
		}

		//! Destructs a forest.
		/*!
		Time complexity: O(that.set() + that.size())
		Exception safety: nothrow
		*/
		~RedBlackForest() = default;

		//! Removes all elements and sets.
		/*!
		Time complexity: O(sets() + size())
		Exception safety: nothrow
		*/
		void clear()
		{
			Set_Iterator set = csetBegin();
			Set_Iterator end = csetEnd();
			while (set != end)
			{
				set = eraseSet(set);
			}
		}

		//! Swaps two forests.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(RedBlackForest& that)
		{
			setSet_.swap(that.setSet_);
		}

		//! Inserts an empty set at the end of the forest.
		/*!
		This is a convenience function which calls
		insertSet(csetEnd()).
		*/
		Set_Iterator insert()
		{
			return insert(csetEnd());
		}

		//! Inserts an empty set into the forest.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		Set_Iterator insert(
			const Set_ConstIterator& before)
		{
			Set_Iterator set = setSet_.emplace(before);
			set->end().sentinelData().set = set;
			return set;
		}

		//! Removes a set from the forest.
		/*!
		Time complexity: O(set->size())
		Exception safety: nothrow
		*/
		Set_Iterator erase(const Set_ConstIterator& set)
		{
			return setSet_.erase(set);
		}

		//! Finds the set that contains a given element.
		/*!
		Time complexity: 
		O(log(n + 2))
		where
		n is the number of elements in the set.

		Exception safety: nothrow
		*/
		Set_ConstIterator find(
			const ConstIterator& element) const
		{
			while(!element.isSentinel())
			{
				element = element.parent();
			}

			return element.sentinelData().set();
		}

		//! Finds the set that contains a given element.
		/*!
		This is a convenience function which returns
		cast(addConst(*this).findSet(element));
		*/
		Set_Iterator find(
			const ConstIterator& element)
		{
			return cast(addConst(*this).find(element));
		}

		//! Returns the number of sets in the forest.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer sets() const
		{
			return setSet_.size();
		}

		//! Removes constness from an iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator cast(const ConstIterator& that)
		{
			return setBegin()->cast(that);
		}

		//! Removes constness from a set-iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Set_Iterator cast(const Set_ConstIterator& that)
		{
			return setSet_.erase(that, that);
		}

		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Set_, setBegin, std::next(setSet_.begin()));
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Set_, setEnd, std::prev(setSet_.end()));
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Set_, setLast, std::prev(setEnd()));

		PASTEL_ITERATOR_FUNCTIONS(begin, setBegin()->begin());
		PASTEL_ITERATOR_FUNCTIONS(end, setEnd()->end());
		PASTEL_ITERATOR_FUNCTIONS(last, setEnd()->last());
		PASTEL_RANGE_FUNCTIONS(range, begin, end);

	private:
		SetSet setSet_;
	};

}

namespace Pastel
{

	template <typename RedBlackTree_>
	class RedBlackForest_Settings
	{
	public:
		using ElementSet = RedBlackTree_;
	};

}

namespace Pastel
{

	template <typename RedBlackTree_,
		template <typename> class Customization = Empty_RedBlackForest_Customization>
	using RedBlackForest_Set = 
		RedBlackForest<RedBlackForest_Settings<RedBlackTree_>, 
		Customization>;

}

#endif
