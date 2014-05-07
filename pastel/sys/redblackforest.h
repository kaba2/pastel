// Description: Red-black forest

#ifndef PASTELSYS_REDBLACKFOREST_H
#define PASTELSYS_REDBLACKFOREST_H

#include "pastel/sys/redblackforest_concepts.h"
#include "pastel/sys/redblackforest_fwd.h"
#include "pastel/sys/redblackforest_set.h"

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

		PASTEL_FWD(Key);
		PASTEL_FWD(Data_Class);

		PASTEL_FWD(ElementSet);
		PASTEL_FWD(Element_Iterator);
		PASTEL_FWD(Element_ConstIterator);

		PASTEL_FWD(SetSet);
		PASTEL_FWD(Set_Iterator);
		PASTEL_FWD(Set_ConstIterator);

		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);

		using Customization = Customization_<Settings>;

		RedBlackForest()
		: setSet_()
		, size_(0)
		{
			// Create the first set.
			setSet_.emplace_back();
			// Create the last set.
			setSet_.emplace_back();
		}

		RedBlackForest(const RedBlackForest& that) = delete;
		RedBlackForest(RedBlackForest&& that) = delete;
		RedBlackForest& operator=(RedBlackForest) = delete;
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

			size_ = 0;
		}

		//! Swaps two forests.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(RedBlackForest& that)
		{
			setSet_.swap(that.setSet_);
			std::swap(size_, that.size_);
		}

		//! Inserts an empty last set into the forest.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		Set_Iterator insertSet()
		{
			return insertSet(csetEnd());
		}

		//! Inserts an empty set into the forest.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		Set_Iterator insertSet(
			const Set_ConstIterator& before)
		{
			return setSet_.emplace(before);
		}

		//! Removes a set from the forest.
		/*!
		Time complexity: O(set->size())
		Exception safety: nothrow
		*/
		Set_Iterator eraseSet(const Set_ConstIterator& set)
		{
			return setSet_.erase(set);
		}

		//! Splits a set into two along the given key.
		/*!
		Time complexity: O(set->size())
		Exception safety: strong
		*/
		Set_Iterator split(
			const Set_ConstIterator& set,
			const Key& key)
		{
			Set_Iterator nextSet;

			// Split the element-set of 'set'.
			ElementSet nextElementSet = cast(set)->split(key);

			try
			{
				// Create a new set.
				nextSet = insertSet(set);

				// Move the splitted element-set into the new set.
				nextSet->elementSet_ = std::move(nextElementSet);
			}
			catch(...)
			{
				// Roll back the set-split.
				cast(set)->join(nextElementSet);
				throw;
			};

			// Return the new set.
			return nextSet;
		}

		//! Joins the elements in 'fromSet' to 'toSet'.
		/*!
		Time complexity: 
		O(log(n_1 + 2) - log(n_2 + 2))
		where
		n_1 = max(fromSet->size(), toSet->size()), and
		n_2 = min(fromSet->size(), toSet->size()).

		Exception safety: nothrow
		*/
		void join(
			const Set_ConstIterator& fromSet,
			const Set_ConstIterator& toSet)
		{
			cast(toSet)->join(cast(fromSet)->elementSet_);
		}

		//! Finds the set that contains a given element.
		/*!
		Time complexity: 
		O(log(n + 2))
		where
		n is the number of elements in the set.

		Exception safety: nothrow
		*/
		Set_ConstIterator findSet(
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
		Set_Iterator findSet(
			const ConstIterator& element)
		{
			return cast(addConst(*this).findSet(element));
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

		//! Inserts an element into a set.
		/*!
		Time complexity: O(log(set->size() + 2))
		Exception safety: strong
		*/
		std::pair<Iterator, bool> insert(
			const Set_ConstIterator& set,
			const Key& key, 
			const Data_Class& data = Data_Class())
		{
			auto result = cast(set)->insert(key, data);
			++size_;
			return result;
		}

		//! Removes an element from a set.
		/*!
		Time complexity: O(log(set->size() + 2))
		Exception safety: nothrow
		*/
		Iterator erase(
			const ConstIterator& element)
		{
			Set_Iterator set = findSet(element);
			--size_;
			return set->erase(element);
		}

		//! Returns whether a set contains a given key.
		/*!
		Time complexity: O(log(set->size()))
		Exception safety: nothrow
		*/
		bool exists(
			const Set_ConstIterator& set,
			const Key& key) const
		{
			return set->exists(key);
		}

		//! Searches for an element with a given key.
		/*!
		Time complexity: O(log(set->size()))
		Exception safety: nothrow
		*/
		ConstIterator find(
			const Set_ConstIterator& set,
			const Key& key) const
		{
			return set->find(key);
		}

		//! Searches for an element with a given key.
		/*!
		This is a convenience function which calls
		cast(addConst(*this).find(set, key));
		*/
		Iterator find(
			const Set_ConstIterator& set,
			const Key& key)
		{
			return cast(addConst(*this).find(set, key));
		}

		//! Returns the smallest element >= key.
		/*!
		Time complexity: O(log(set->size()))
		Exception safety: nothrow
		*/
		ConstIterator lowerBound(
			const Set_ConstIterator& set,
			const Key& key) const
		{
			return set->lowerBound(key);
		}

		//! Returns the smallest element >= key.
		/*!
		This is a convenience function which calls
		cast(addConst(*this).lowerBound(set, key));
		*/
		Iterator lowerBound(
			const Set_ConstIterator& set,
			const Key& key)
		{
			return cast(addConst(*this).lowerBound(set, key));
		}

		//! Returns the smallest element > key.
		/*!
		Time complexity: O(log(set->size()))
		Exception safety: nothrow
		*/
		ConstIterator upperBound(
			const Set_ConstIterator& set,
			const Key& key) const
		{
			return set->upperBound(key);
		}

		//! Returns the smallest element > key.
		/*!
		This is a convenience function which calls
		cast(addConst(*this).upperBound(set, key));
		*/
		Iterator upperBound(
			const Set_ConstIterator& set,
			const Key& key)
		{
			return cast(addConst(*this).upperBound(set, key));
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

		//! Returns the number of elements in the forest.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer size() const
		{
			return size_;
		}

		//! Returns whether there are elements in the forest.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool empty() const
		{
			return size_ == 0;
		}

		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Set_, setBegin, std::next(setSet_.begin()));
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Set_, setEnd, std::prev(setSet_.end()));
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Set_, setLast, std::prev(setEnd()));

		PASTEL_ITERATOR_FUNCTIONS(begin, setBegin()->begin());
		PASTEL_ITERATOR_FUNCTIONS(end, setEnd()->end());
		PASTEL_ITERATOR_FUNCTIONS(last, setEnd()->last());

	private:
		SetSet setSet_;
		integer size_;
	};

}

namespace Pastel
{

	template <
		typename Key_, 
		typename Data_ = void,
		typename Less_ = LessThan,
		typename Propagation_ = void,
		typename SentinelData_ = void,
		bool MultipleKeys_ = false>
	class RedBlackForest_Settings
	{
	public:
		using Key = Key_;
		using Data = Data_;
		using Less = Less_;
		using Propagation = Propagation_;
		using SentinelData = SentinelData_;
		PASTEL_CONSTEXPR bool MultipleKeys = MultipleKeys_;
	};

}

// Map

namespace Pastel
{

	template <
		typename Key, 
		typename Data,
		typename Less = LessThan,
		typename Propagation = void,
		typename SentinelData = void,
		template <typename> class Customization = Empty_RedBlackForest_Customization>
	using RedBlackForest_Map = 
		RedBlackForest<RedBlackForest_Settings<Key, Data, Less, Propagation, SentinelData, false>, 
		Customization>;

	template <
		typename Key, 
		typename Data,
		typename Less = LessThan,
		typename Propagation = void,
		typename SentinelData = void,
		template <typename> class Customization = Empty_RedBlackForest_Customization>
	using RedBlackForest_MultiMap = 
		RedBlackForest<RedBlackForest_Settings<Key, Data, Less, Propagation, SentinelData, true>, 
		Customization>;

}

// Set

namespace Pastel
{

	template <
		typename Key,
		typename Less = LessThan,
		typename Propagation = void,
		typename SentinelData = void,
		template <typename> class Customization = Empty_RedBlackForest_Customization>
	using RedBlackForest_Set = 
		RedBlackForest<RedBlackForest_Settings<Key, void, Less, Propagation, SentinelData, false>, 
		Customization>;

	template <
		typename Key, 
		typename Less = LessThan,
		typename Propagation = void,
		typename SentinelData = void,
		template <typename> class Customization = Empty_RedBlackForest_Customization>
	using RedBlackForest_MultiSet = 
		RedBlackForest<RedBlackForest_Settings<Key, void, Less, Propagation, SentinelData, true>, 
		Customization>;

}

#endif
