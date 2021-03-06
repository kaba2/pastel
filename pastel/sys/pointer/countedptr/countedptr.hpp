#ifndef PASTELSYS_COUNTEDPTR_HPP
#define PASTELSYS_COUNTEDPTR_HPP

#include "pastel/sys/pointer/countedptr.h"

#include "pastel/sys/ensure.h"

#include <type_traits>

namespace Pastel
{

	template <typename Type>
	CountedPtr<Type>::CountedPtr()
		: data_(0)
	{
	}

	template <typename Type>
	CountedPtr<Type>::CountedPtr(const CountedPtr<Type>& that)
		: data_(that.data_)
	{
		if (data_)
		{
			increaseCount();
		}
	}

	template <typename Type>
	template <typename ThatType>
	CountedPtr<Type>::CountedPtr(const WeakPtr<ThatType>& that)
		: data_(that.get())
	{
		if (data_)
		{
			increaseCount();
		}
	}

	template <typename Type>
	template <typename ThatType>
	CountedPtr<Type>::CountedPtr(
		const CountedPtr<ThatType>& that)
		: data_(that.data_)
	{
		if (data_)
		{
			increaseCount();
		}
	}

	template <typename Type>
	CountedPtr<Type>::CountedPtr(Type* that)
		: data_(that)
	{
		if (data_)
		{
			increaseCount();
		}
	}

	template <typename Type>
	CountedPtr<Type>& CountedPtr<Type>::operator=(
		const CountedPtr<Type>& that)
	{
		CountedPtr<Type> copy(that);
		swap(copy);
		return *this;
	}

	template <typename Type>
	template <typename ThatType>
	CountedPtr<Type>& CountedPtr<Type>::operator=(
		const CountedPtr<ThatType>& that)
	{
		CountedPtr<Type> copy(that);
		swap(copy);
		return *this;
	}

	template <typename Type>
	CountedPtr<Type>& CountedPtr<Type>::operator=(Type* that)
	{
		CountedPtr<Type> copy(that);
		swap(copy);
		return *this;
	}

	template <typename Type>
	CountedPtr<Type>::~CountedPtr()
	{
		clear();
	}

	template <typename Type>
	Type* CountedPtr<Type>::get() const
	{
		return data_;
	}

	template <typename Type>
	Type* CountedPtr<Type>::operator->() const
	{
		PENSURE(!empty());

		return data_;
	}

	template <typename Type>
	Type& CountedPtr<Type>::operator*() const
	{
		PENSURE(!empty());

		return *data_;
	}

	template <typename Type>
	void CountedPtr<Type>::swap(CountedPtr<Type>& that)
	{
		std::swap(data_, that.data_);
	}

	template <typename Type>
	void CountedPtr<Type>::clear()
	{
		if (data_)
		{
			//#pragma omp critical (CountedPtr_ReadWrite)
			{
			// Decrease the reference count.
	
			decreaseCount();

			// If the reference count reaches zero,
			// free the object.

			if (count() == 0)
			{
				delete data_;
			}

			data_ = 0;
			}
		}
	}

	template <typename Type>
	integer CountedPtr<Type>::count() const
	{
		static constexpr bool IsBase =
			std::is_base_of<ReferenceCounted, Type>::value;

		PASTEL_STATIC_ASSERT(IsBase);

		if (!data_)
		{
			return 0;
		}

		const ReferenceCounted* counter =
			(const ReferenceCounted*)data_;
		integer result = 0;
#pragma omp critical (CountedPtr_ReadWrite)
		{
			result = counter->count_;
		}
		return result;
	}

	template <typename Type>
	bool CountedPtr<Type>::empty() const
	{
		return (data_ == 0);
	}

	// Private

	template <typename Type>
	void CountedPtr<Type>::increaseCount() const
	{
		static constexpr bool IsBase =
			std::is_base_of<ReferenceCounted, Type>::value;

		PASTEL_STATIC_ASSERT(IsBase);

		PENSURE(data_);

		const ReferenceCounted* counter =
			(const ReferenceCounted*)data_;

#pragma omp critical (CountedPtr_ReadWrite)
		{
			++(counter->count_);
		}
	}

	template <typename Type>
	void CountedPtr<Type>::decreaseCount() const
	{
		static constexpr bool IsBase =
			std::is_base_of<ReferenceCounted, Type>::value;

		PASTEL_STATIC_ASSERT(IsBase);

		PENSURE(data_);
		ENSURE_OP(count(), >, 0);

		const ReferenceCounted* counter =
			(const ReferenceCounted*)data_;

#pragma omp critical (CountedPtr_ReadWrite)
		{
			--(counter->count_);
		}
	}

	template <typename Type>
	void swap(CountedPtr<Type>& left,
		CountedPtr<Type>& right)
	{
		left.swap(right);
	}

}

#endif
