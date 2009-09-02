// Description: SmallMap class
// Detail: An efficient associative array for small maps
// Documentation: small_containers.txt

#ifndef PASTEL_SMALLMAP_H
#define PASTEL_SMALLMAP_H

#include "pastel/sys/mytypes.h"

#include <vector>
#include <functional>

namespace Pastel
{

	//! A random-access associative array.
	/*!
	The elements of the map are stored in an array
	in ascending order, with the ordering given
	by the < operator of the KeyType.
	Because of the ordering, the find operations 
	can be performed in logarithmic time which makes 
	for fast membership testing. 
	The use of an array makes for a very compact
	memory footprint, and with a small number of objects
	this class is likely to outperform link-based trees
	(depending on how costly operations copying the 
	KeyType and the ValueType are).
	This is the intended usage of this class.
	It should not be used with large number
	of objects, because insert and erase operations 
	have linear complexities.
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

			bool operator<(const Element& that) const
			{
				Compare compare_;
				return compare_(key_, that.key_);
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
