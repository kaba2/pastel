// Description: IndexAllocator class
// Detail: Allocates integer identifiers conservatively

#ifndef PASTELSYS_INDEXALLOCATOR_H
#define PASTELSYS_INDEXALLOCATOR_H

#include "pastel/sys/mytypes.h"
#include <vector>

namespace Pastel
{

	//! Allocates integer identifiers conservatively.
	class IndexAllocator
	{
	public:
		// Using default copy constructor
		// Using default destructor
		// Using default assignment

		//! Default constructor.
		/*!
		Time complexity: constant
		Exception safety: strong
		*/
		IndexAllocator();

		//! Standard swap.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void swap(IndexAllocator& that);

		//! Deallocates all indices.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void clear();

		//! Preallocates memory for indices.
		/*!
		Preconditions:
		count >= 0

		Time complexity: linear
		Exception safety: strong
		*/
		void reserve(integer count);

		//! Returns the number of allocated indices.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer size() const;

		//! Returns the number of allocatable indices without a reallocation.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer capacity() const;

		//! Checks if an identifier has already been allocated.
		/*!
		Preconditions:
		0 <= index < size()

		Time complexity: constant
		Exception safety: nothrow
		*/
		bool isAllocated(integer index) const;

		//! Finds the next allocated index.
		/*!
		Preconditions:
		0 <= index < size()

		Time complexity: logarithmic
		Exception safety: nothrow
		*/
		integer nextIndex(integer index) const;

		//! Allocates a new identifier.
		/*!
		Time complexity: amortized logarithmic
		Exception safety: strong
		*/
		integer allocate();

		//! Deallocates an identifier.
		/*!
		Time complexity: logarithmic
		Exception safety: nothrow
		*/
		void deallocate(integer index);

	private:
		bool isLeaf(integer index) const;
		integer leftChild(integer index) const;
		integer rightChild(integer index) const;
		integer parent(integer index) const;
		integer toInternalIndex(integer index) const;
		integer toExternalIndex(integer index) const;

		void grow(integer toGrow);
		void update(integer index);
		integer findFree() const;

		std::vector<bool> data_;
		integer allocated_;
		integer heapHeight_;
	};

	void swap(IndexAllocator& left, IndexAllocator& right);

}

#include "pastel/sys/indexallocator.hpp"

#endif
