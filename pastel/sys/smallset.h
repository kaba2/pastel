/*!
\file
\brief A class for a small random-access set.
*/

#ifndef PASTELSYS_SMALLSET_H
#define PASTELSYS_SMALLSET_H

#include "pastel/sys/mytypes.h"

#include <vector>
#include <functional>

namespace Pastel
{

	//! A random-access contiguous memory set.
	/*!
	The elements of the set are stored in an array
	in ascending order, with the ordering given
	by the < operator of the Type.
	Because of the ordering, the find operations 
	can be performed in logarithmic time which makes 
	for fast membership testing. 
	The use of an array makes for a very compact
	memory footprint, and with a small number of objects
	this class is likely to outperform link-based trees
	(depending on how costly an operation copying the Type is).
	This is the intended usage of this class.
	It should not be used with large number
	of objects, because insert and erase operations 
	have linear complexities.
	*/

	template <typename Type, typename Compare = std::less<Type> >
	class SmallSet
	{
	private:
		typedef std::vector<Type> Container;
		typedef typename Container::iterator Iterator;
		typedef typename Container::const_iterator ConstIterator;

		class EmptyAction
		{
		public:
			void operator()(const Type& left, const Type& right) const
			{
			}
		};

	public:
		//! Constructs an empty set.
		/*!
		Time complexity: constant.
		Exception safety: strong.
		*/
		explicit SmallSet(const Compare& compare = Compare());

		//! Constructs with a copy of another set.
		/*!
		Time complexity: linear.
		Exception safety: strong.
		*/
		SmallSet(const SmallSet& that);

		//! Destructs the set.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		~SmallSet();

		//! Assigns a copy of another set.
		/*!
		Time complexity: linear.
		Exception safety: strong.
		*/
		SmallSet<Type, Compare>& operator=(const SmallSet& that);

		//! Swaps two sets.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		void swap(SmallSet& that);

		//! Clears the set.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		void clear();

		//! Reserves memory in advance to avoid reallocation.
		/*!
		Time complexity: linear, constant if capacity() >= count.
		Exception safety: strong.
		*/
		void reserve(integer count);

		//! Changes the value of an element.
		/*!
		Time complexity: linear in the number of moves needed.
		Exception safety: strong.
		*/
		integer set(integer index, const Type& that);

		//! Insert an element into the set.
		/*!
		Time complexity: linear, constant if the greatest element.
		Exception safety: strong.
		*/
		integer insert(const Type& that);

		//! Removes an element from the set.
		/*!
		Time complexity: linear.
		Exception safety: nothrow.
		*/
		void erase(const Type& that);

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
		integer find(const Type& that) const;

		//! Finds the index of the lower bound to the element.
		/*!
		Time complexity: logarithmic.
		Exception safety: nothrow.
		*/
		integer lower_bound(const Type& that) const;

		//! Finds the index of the upper bound to the element.
		/*!
		Time complexity: logarithmic.
		Exception safety: nothrow.
		*/
		integer upper_bound(const Type& that) const;

		//! Returns the index:th smallest element of the set.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		Type& operator[](integer index);

		//! Returns the index:th smallest element of the set.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		const Type& operator[](integer index) const;

		//! Returns the smallest element of the set.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		const Type& front() const;

		//! Returns the largest element of the set.
		/*!
		Time complexity: constant.
		Exception safety: nothrow.
		*/
		const Type& back() const;

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
		std::vector<Type> data_;
		Compare compare_;
	};

	template <typename Type, typename Compare>
	void swap(SmallSet<Type, Compare>& left,
		SmallSet<Type, Compare>& right);

}

#include "pastel/sys/smallset.hpp"

#endif
