// Description: Smart array pointer
// Documentation: memory.txt

#ifndef PASTELSYS_ARRAYPTR_H
#define PASTELSYS_ARRAYPTR_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Type>
	class ArrayPtr
	{
	public:
		//! Constructs an empty array-pointer.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		ArrayPtr()
		: data_(0)
		, size_(0)
		{
		}

		//! Array-pointers can not be copied.
		ArrayPtr(const ArrayPtr& that) = delete;

		//! Move-constructs an array-pointer.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		ArrayPtr(ArrayPtr&& that)
		: ArrayPtr()
		{
			swap(that);
		}

		//! Constructs from a raw-array-pointer and size.
		/*!
		Preconditions:
		data has been allocated with new[]
		size >= 0

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		ArrayPtr(Type* data, integer size)
		: data_(data)
		, size_(size)
		{
			PENSURE_OP(size, >=, 0);
		}

		//! Destructs an array-pointer.
		/*!
		Calls clear().

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		~ArrayPtr()
		{
			clear();
		}

		explicit operator bool() const
		{
			return data_ != 0;
		}

		//! Assigning is not allowed.
		ArrayPtr& operator=(const ArrayPtr& that) = delete;

		//! Move-assigns from another array-pointer.
		/*!
		The memory stored in the array-pointer is deallocated
		with delete[].

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		ArrayPtr& operator=(ArrayPtr&& that)
		{
			ArrayPtr moved(std::move(that));
			swap(moved);
			return *this;
		}

		//! Deallocates stored memory.
		/*!
		The memory stored is deallocated using delete[].

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void clear()
		{
			if (data_)
			{
				delete[] data_;
			}
			data_ = 0;
			size_ = 0;
		}

		//! Sets the stored raw-array-pointer and size.
		/*!
		Preconditions:
		data has been allocated with new[]
		size >= 0

		The existing memory is deallocated with clear().

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void set(Type* data, integer size)
		{
			PENSURE_OP(size, >=, 0);
			swap(ArrayPtr(data, size));
		}

		//! Returns the stored raw-array-pointer.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Type* get()
		{
			return data_;
		}

		//! Returns the stored raw-array-pointer.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		const Type* get() const
		{
			return data_;
		}

		//! Swaps two array-pointers.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(ArrayPtr& that)
		{
			std::swap(data_, that.data_);
			std::swap(size_, that.size_);
		}

		//! Returns the element at index i.
		/*!
		See the documentation for const version.
		*/
		Type& operator[](integer i)
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, size());
			return data_[i];
		}

		//! Returns the element at index i.
		/*!
		Preconditions:
		0 <= i < size()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		const Type& operator[](integer i) const
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, size());
			return data_[i];
		}

		//! Resizes the array.
		/*!
		Preconditions:
		size >= 0

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void resize(integer size)
		{
			ENSURE_OP(size, >=, 0);
			size_ = size;
		}

		//! Returns the size of the array.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer size() const
		{
			return size_;
		}

	private:
		Type* data_;
		integer size_;
	};
	
}

#endif
