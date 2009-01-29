/*!
\file
\brief A class for an associative array.
*/

#ifndef PASTELSYS_SMALLMAP_H
#define PASTELSYS_SMALLMAP_H

#include "pastel/sys/mytypes.h"

#include <vector>
#include <functional>

namespace Pastel
{

	//! A random-access associative array.

	/*!
	Requirements for ValueType:
	* Default-constructible

	This class is meant to handle the need for a map
	which can still be handled like an array.
	The elements of the map are stored in contiguous
	memory in ascending order. The ordering is given
	by the < operator of the Key.
	These properties come at a price:
	insert and erase operations have linear complexity.
	However the largest element can be inserted and
	erased (via pop_back()) in constant time.
	The find operations are logarithmic which
	make for fast membership testing. Because of
	the asymptotically slow insert and erase operations,
	this class should only be used with a small amount of
	elements to avoid	performance problems. On the other
	hand, because the operations are simple and the memory
	contiguous, the constant and linear factors are
	very low. Thus with small maps this class is likely
	to outperform link-based maps (depending on how
	costly operation copying the ValueType is).
	*/

	template <typename KeyType, typename ValueType, typename Compare = std::less<KeyType> >
	class SmallMap
	{
	private:
		struct Element
		{
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			Element()
				: key_()
				, value_()
			{
			}

			Element(const KeyType& key,
				const ValueType& value)
				: key_(key)
				, value_(value)
			{
			}

			KeyType key_;
			ValueType value_;
		};

		typedef std::vector<Element> Container;
		typedef typename Container::iterator Iterator;
		typedef typename Container::const_iterator ConstIterator;

	public:
		//! Constructs an empty set.
		/*!
		Time complexity: constant.
		Exception safety: strong.
		*/
		SmallMap(const Compare& compare = Compare());

		//! Constructs with a copy of another set.
		/*!
		Time complexity: linear.
		Exception safety: strong.
		*/
		SmallMap(const SmallMap& that);

		//! Constructs with a copy of another set.
		/*!
		Time complexity: linear.
		Exception safety: strong.
		*/
		SmallMap(const SmallMap& that,
			const Compare& compare);

		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		~SmallMap();

		//! Assigns a copy of another set.
		/*!
		Time complexity: linear.
		Exception safety: strong.
		*/
		SmallMap<KeyType, ValueType, Compare>& operator=(
			const SmallMap& that);

		//! Swaps two sets.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		void swap(SmallMap& that);

		//! Clears the set.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		void clear();

		//! Reserves memory in advantage to avoid reallocation.
		/*!
		Time complexity: linear, constant if capacity() >= count.
		Exception safety: strong.
		*/
		void reserve(integer count);

		//! Insert an element into the set.
		/*!
		Time complexity: linear, constant if the greatest element.
		Exception safety: strong.
		*/
		integer insert(const KeyType& key,
			const ValueType& value);

		//! Removes an element from the set.
		/*!
		Time complexity: linear.
		Exception safety: nothrow.
		*/
		void erase(const KeyType& that);

		//! Removes an element from the set.
		/*!
		Time complexity: linear.
		Exception safety: nothrow.
		*/
		void eraseFrom(integer index);

		//! Removes the largest element from the set.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		void pop_back();

		//! Finds the index of the given element.
		/*!
		Time complexity: logarithmic.
		Exception safety: nothrow.
		*/
		integer find(const KeyType& that) const;

		//! Finds the index of the lower bound to the element.
		/*!
		Time complexity: logarithmic.
		Exception safety: nothrow.
		*/
		integer lower_bound(const KeyType& that) const;

		//! Finds the index of the upper bound to the element.
		/*!
		Time complexity: logarithmic.
		Exception safety: nothrow.
		*/
		integer upper_bound(const KeyType& that) const;

		//! Returns the index:th smallest key.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		const KeyType& keyAt(integer index) const;

		//! Returns the value of the index:th smallest key.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		ValueType& valueAt(integer index);

		//! Returns the value of the index:th smallest key.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		const ValueType& valueAt(integer index) const;

		//! Returns the smallest key.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		const KeyType& keyFront() const;

		//! Returns the largest key.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		const KeyType& keyBack() const;

		//! Returns the value of the smallest key.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		ValueType& valueFront();

		//! Returns the value of the smallest key.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		const ValueType& valueFront() const;

		//! Returns the value of the largest key.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		ValueType& valueBack();

		//! Returns the value of the largest key.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		const ValueType& valueBack() const;

		//! Returns true if the set is empty, false otherwise.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		bool empty() const;

		//! Returns the number of elements in the set.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		integer size() const;

		//! Returns the current reallocation limit.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		integer capacity() const;

	private:
		std::vector<Element> data_;
		Compare compare_;
	};

}

#include "pastel/sys/smallmap.hpp"

#endif
