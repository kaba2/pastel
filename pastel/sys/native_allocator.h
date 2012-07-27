// Description: Native allocator
// Detail: An allocator wrapper for new and delete

#ifndef PASTEL_NATIVE_ALLOCATOR_H
#define PASTEL_NATIVE_ALLOCATOR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! A uniform allocator wrapper for new and delete.

	class PASTELSYS NativeAllocator
		: boost::equality_comparable<
		NativeAllocator
		, boost::less_than_comparable<
		NativeAllocator
		> >
	{
	public:
		//! Constructs an allocator with the given 'unitSize'.
		/*!
		Time complexity: constant
		Exception safety: nothrow

		The parameter 'isPodDataIgnored' has no meaning with
		this allocator.
		*/
		explicit NativeAllocator(
			integer unitSize,
			bool isPodDataIgnored = false);

		//! Destructs the allocator.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		~NativeAllocator();

		//! Compares two allocators.
		/*!
		Time complexity: constant
		Exception safety: nothrow

		The comparison is done by memory addresses.
		*/
		bool operator==(const NativeAllocator& that) const;

		//! Compares two allocators.
		/*!
		Time complexity: constant
		Exception safety: nothrow

		The comparison is done by memory addresses.
		*/
		bool operator<(const NativeAllocator& that) const;

		//! Swaps two allocators.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void swap(NativeAllocator& that);

		//! A no-op.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void clear();

		//! Returns the unit size.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer unitSize() const;

		//! Returns the amount of requested memory.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer size() const;

		//! Returns the actual amount of allocated memory.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer capacity() const;

		//! Allocates unitSize-sized memory area.
		/*!
		Time complexity: constant
		Exception safety: strong
		*/
		void* allocate();

		//! Deallocates a previously allocated area.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void deallocate(const void* memAddress);

	private:
		NativeAllocator(const NativeAllocator& that) PASTEL_DELETE;
		NativeAllocator& operator=(const NativeAllocator& that) PASTEL_DELETE;

		integer unitSize_;
		integer unitsAllocated_;
	};

	void swap(NativeAllocator& left, NativeAllocator& right);

}

#include "pastel/sys/native_allocator.hpp"

#endif
