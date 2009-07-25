// Description: MemoryView class
// Detail: A View interface to a memory range.

#ifndef PASTEL_MEMORYVIEW_H
#define PASTEL_MEMORYVIEW_H

#include "pastel/sys/mytypes.h"

#include <memory>

#include <boost/operators.hpp>

namespace Pastel
{

	//! Allows View interface to a memory range.

	template <typename Type>
	class MemoryView
		: boost::less_than_comparable<
		MemoryView<Type>
		, boost::equality_comparable<
		MemoryView<Type>
		> >
	{
	public:
		typedef Type Element;

		template <typename Other>
		friend class MemoryView;

		//! Constructs an empty MemoryView.
		MemoryView();

		//! Constructs using the given parameters.
		MemoryView(Type* data, integer nSize);

		//! Returns if the arrays point to the same memory.
		bool operator==(const MemoryView<Type>& that) const;

		//! Compares two array views.
		bool operator<(const MemoryView<Type>& that) const;

		//! Returns the index:th object in the array.
		Type& operator[](integer index) const;

		//! Swaps two array views.
		void swap(MemoryView<Type>& That);

		//! Sets the starting memory address and size of the array.
		void set(Type* data, integer nSize);

		//! Returns the size of the array.
		integer size() const;

		//! Sets the memory address to null and size to zero.
		void clear();

		//! Returns true if the size is zero.
		bool empty() const;

		//! Returns an iterator to the first element.
		Type* begin() const;

		//! Returns an iterator to the one-past-last element.
		Type* end() const;

	private:
		Type* data_;
		integer size_;
	};

	template <typename Type>
	void swap(MemoryView<Type>& left, MemoryView<Type>& right);

	//! Allows View interface to a memory range.

	template <typename Type>
	class ConstMemoryView
		: boost::less_than_comparable<
		ConstMemoryView<Type>
		, boost::equality_comparable<
		ConstMemoryView<Type>
		> >
	{
	public:
		typedef Type Element;

		//! Constructs an empty ConstMemoryView.
		ConstMemoryView();

		//! Constructs from MemoryView
		// Implicit conversion allowed.
		ConstMemoryView(const MemoryView<Type>& that);

		//! Constructs using the given parameters.
		ConstMemoryView(const Type* data, integer nSize);

		//! Returns if the arrays point to the same memory.
		bool operator==(const ConstMemoryView<Type>& that) const;

		//! Compares two array views.
		bool operator<(const ConstMemoryView<Type>& that) const;

		//! Returns the index:th object in the array.
		const Type& operator[](integer index) const;

		//! Swaps two array views.
		void swap(ConstMemoryView<Type>& That);

		//! Sets the starting memory address and size of the array.
		void set(const Type* data, integer nSize);

		//! Returns the size of the array.
		integer size() const;

		//! Sets the memory address to null and size to zero.
		void clear();

		//! Returns true if the size is zero.
		bool empty() const;

		//! Returns an iterator to the first element.
		const Type* begin() const;

		//! Returns an iterator to the one-past-last element.
		const Type* end() const;

	private:
		const Type* data_;
		integer size_;
	};

	template <typename Type>
	void swap(ConstMemoryView<Type>& left, ConstMemoryView<Type>& right);

}

#include "pastel/sys/memoryview.hpp"

#endif
