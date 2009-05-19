/*!
\file
\brief A class for allocating integer identifiers.
*/

#ifndef PASTEL_INDEXALLOCATOR_H
#define PASTEL_INDEXALLOCATOR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"
#include <vector>

namespace Pastel
{

	//! Allocates integer identifiers conservatively.

	/*!
	This class is used to allocate and deallocate integer
	identifiers. The integers range from 0 to an arbitrary number.
	At all times the class has a "capacity". Capacity gives the
	amount of identifiers that the class can allocate before
	a reallocation of its internal memory is needed. The
	allocation scheme is conservative: every free identifier will
	be allocated before forcing a capacity change. The
	allocations and deallocations on this data structure
	are amortized logarithmic and logarithmic. The implementation
	uses a heap binary tree stored in an array.
	Every node of this tree describes the state of its subtree:
	if all subnodes of the node are all allocated, then the node
	is also allocated, otherwise the node is free.
	You can visualize the heap as follows:
	take all the leaf nodes and arrange them in a row. This
	is the actual identifier allocation information. Every subsequent
	pair of this row have a common parent. Replace these pairs with
	their parents and again arrange them in a row. This row is exactly
	half the width of the original. For a given pair, if both nodes
	are allocated, then the parent is also marked allocated, otherwise the
	parent is marked free. If you repeat in such a way all the way to the
	root, you end up producing a shortening sequence of "blurred"
	allocation information. In the root all the information has been
	blurred in a single value. This data structure can be
	easily used for logarithmic searches of free nodes as
	well as modifying a node and updating the heap in logarithmic time.
	*/

	class PASTELSYS IndexAllocator
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
