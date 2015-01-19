#ifndef PASTELSYS_WEAKPTR_HPP
#define PASTELSYS_WEAKPTR_HPP

#include "pastel/sys/pointer/weakptr.h"

#include "pastel/sys/ensure.h"

#include <type_traits>

namespace Pastel
{

	template <typename Type>
	WeakPtr<Type>::WeakPtr()
		: data_(0)
	{
	}

	template <typename Type>
	WeakPtr<Type>::WeakPtr(const WeakPtr<Type>& that)
		: data_(that.data_)
	{
	}

	template <typename Type>
	template <typename ThatType>
	WeakPtr<Type>::WeakPtr(const CountedPtr<ThatType>& that)
		: data_(that.get())
	{
	}

	template <typename Type>
	template <typename ThatType>
	WeakPtr<Type>::WeakPtr(
		const WeakPtr<ThatType>& that)
		: data_(that.data_)
	{
	}

	template <typename Type>
	WeakPtr<Type>::WeakPtr(Type* that)
		: data_(that)
	{
	}

	template <typename Type>
	WeakPtr<Type>& WeakPtr<Type>::operator=(
		const WeakPtr<Type>& that)
	{
		WeakPtr<Type> copy(that);
		swap(copy);
		return *this;
	}

	template <typename Type>
	template <typename ThatType>
	WeakPtr<Type>& WeakPtr<Type>::operator=(
		const WeakPtr<ThatType>& that)
	{
		WeakPtr<Type> copy(that);
		swap(copy);
		return *this;
	}

	template <typename Type>
	WeakPtr<Type>& WeakPtr<Type>::operator=(Type* that)
	{
		WeakPtr<Type> copy(that);
		swap(copy);
		return *this;
	}

	template <typename Type>
	template <typename ThatType>
	bool WeakPtr<Type>::operator==(
		const WeakPtr<ThatType>& that) const
	{
		return data_ == that.data_;
	}

	template <typename Type>
	template <typename ThatType>
	bool WeakPtr<Type>::operator<(
		const WeakPtr<ThatType>& that) const
	{
		return data_ < that.data_;
	}

	template <typename Type>
	WeakPtr<Type>::~WeakPtr()
	{
		clear();
	}

	template <typename Type>
	Type* WeakPtr<Type>::get() const
	{
		return data_;
	}

	template <typename Type>
	Type* WeakPtr<Type>::operator->() const
	{
		PENSURE(!empty());

		return data_;
	}

	template <typename Type>
	Type& WeakPtr<Type>::operator*() const
	{
		PENSURE(!empty());

		return *data_;
	}

	template <typename Type>
	void WeakPtr<Type>::swap(WeakPtr<Type>& that)
	{
		std::swap(data_, that.data_);
	}

	template <typename Type>
	void WeakPtr<Type>::clear()
	{
		data_ = 0;
	}

	template <typename Type>
	integer WeakPtr<Type>::count() const
	{
		static PASTEL_CONSTEXPR bool IsBase = 
			std::is_base_of<ReferenceCounted, Type>::value;
		PASTEL_STATIC_ASSERT(IsBase);

		if (!data_)
		{
			return 0;
		}

		const ReferenceCounted* counter =
			(const ReferenceCounted*)data_;
		return counter->count_;
	}

	template <typename Type>
	bool WeakPtr<Type>::empty() const
	{
		return (data_ == 0);
	}

	template <typename Type>
	void swap(WeakPtr<Type>& left,
		WeakPtr<Type>& right)
	{
		left.swap(right);
	}

}

#endif
