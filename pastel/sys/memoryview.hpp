#ifndef PASTELSYS_MEMORYVIEW_HPP
#define PASTELSYS_MEMORYVIEW_HPP

#include "pastel/sys/memoryview.h"

#include "pastel/sys/ensure.h"

#include <algorithm>

namespace Pastel
{

	// MemoryView

	template <typename Type>
	MemoryView<Type>::MemoryView()
		: data_(0)
		, size_(0)
	{
	}

	template <typename Type>
	MemoryView<Type>::MemoryView(Type* data, integer nSize)
		: data_(data)
		, size_(nSize)
	{
	}

	template <typename Type>
	bool MemoryView<Type>::operator==(
		const MemoryView<Type>& that) const
	{
		return data_ == that.data_;
	}

	template <typename Type>
	bool MemoryView<Type>::operator<(
		const MemoryView<Type>& that) const
	{
		return data_ < that.data_;
	}

	template <typename Type>
	Type& MemoryView<Type>::operator[](integer index) const
	{
		PENSURE2(index >= 0 && index < size_,
			index, size_);

		return data_[index];
	}

	template <typename Type>
	void MemoryView<Type>::swap(MemoryView<Type>& that)
	{
		std::swap(data_, that.data_);
		std::swap(size_, that.size_);
	}

	template <typename Type>
	void MemoryView<Type>::set(Type* data, integer nSize)
	{
		data_ = data;
		size_ = nSize;
	}

	template <typename Type>
	integer MemoryView<Type>::size() const
	{
		return size_;
	}

	template <typename Type>
	void MemoryView<Type>::clear()
	{
		data_ = 0;
		size_ = 0;
	}

	template <typename Type>
	bool MemoryView<Type>::empty() const
	{
		return (size_ == 0);
	}

	template <typename Type>
	Type* MemoryView<Type>::begin() const
	{
		return data_;
	}

	template <typename Type>
	Type* MemoryView<Type>::end() const
	{
		return data_ + size_;
	}

	template <typename Type>
	void swap(MemoryView<Type>& left, MemoryView<Type>& right)
	{
		left.swap(right);
	}

	// ConstMemoryView

	template <typename Type>
	ConstMemoryView<Type>::ConstMemoryView()
		: data_(0)
		, size_(0)
	{
	}

	template <typename Type>
	ConstMemoryView<Type>::ConstMemoryView(
		const MemoryView<Type>& that)
		: data_(that.data_)
		, size_(that.size_)
	{
	}

	template <typename Type>
	ConstMemoryView<Type>::ConstMemoryView(
		const Type* data, integer nSize)
		: data_(data)
		, size_(nSize)
	{
	}

	template <typename Type>
	bool ConstMemoryView<Type>::operator==(
		const ConstMemoryView<Type>& that) const
	{
		return data_ == that.data_;
	}

	template <typename Type>
	bool ConstMemoryView<Type>::operator<(
		const ConstMemoryView<Type>& that) const
	{
		return data_ < that.data_;
	}

	template <typename Type>
	const Type& ConstMemoryView<Type>::operator[](
		integer index) const
	{
		PENSURE2(index >= 0 && index < size_,
			index, size_);

		return data_[index];
	}

	template <typename Type>
	void ConstMemoryView<Type>::swap(
		ConstMemoryView<Type>& that)
	{
		std::swap(data_, that.data_);
		std::swap(size_, that.size_);
	}

	template <typename Type>
	void ConstMemoryView<Type>::set(
		const Type* data, integer nSize)
	{
		data_ = data;
		size_ = nSize;
	}

	template <typename Type>
	integer ConstMemoryView<Type>::size() const
	{
		return size_;
	}

	template <typename Type>
	void ConstMemoryView<Type>::clear()
	{
		data_ = 0;
		size_ = 0;
	}

	template <typename Type>
	bool ConstMemoryView<Type>::empty() const
	{
		return (size_ == 0);
	}

	template <typename Type>
	const Type* ConstMemoryView<Type>::begin() const
	{
		return data_;
	}

	template <typename Type>
	const Type* ConstMemoryView<Type>::end() const
	{
		return data_ + size_;
	}

	template <typename Type>
	void swap(ConstMemoryView<Type>& left,
		ConstMemoryView<Type>& right)
	{
		left.swap(right);
	}

}

#endif
