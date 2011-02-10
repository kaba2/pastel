#ifndef PASTEL_SMALLFIXEDSET_HPP
#define PASTEL_SMALLFIXEDSET_HPP

#include "pastel/sys/smallfixedset.h"

#include "pastel/sys/smallfixedset.h"
#include "pastel/sys/stdext_destruct.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Type>
	SmallFixedSet<Type>::SmallFixedSet(integer capacity)
		: data_(0)
		, capacity_(capacity)
		, size_(0)
	{
		ENSURE_OP(capacity, >=, 0);

		data_ = (Type*)allocateRaw(capacity * sizeof(Type));
	}

	template <typename Type>
	SmallFixedSet<Type>::SmallFixedSet(
		const SmallFixedSet& that)
		: data_(0)
		, capacity_(0)
		, size_(0)
	{
		data_ = (Type*)allocateRaw(that.capacity() * sizeof(Type));
		try
		{
			std::uninitialized_copy(
			that.begin(), that.end(),
			data_);
		}
		catch(...)
		{
			deallocateRaw((void*)data_);
			data_ = 0;
			throw;
		};
		capacity_ = that.capacity_;
		size_ = that.size_;
	}

	template <typename Type>
	SmallFixedSet<Type>::~SmallFixedSet()
	{
		StdExt::destruct(begin(), end());
		deallocateRaw((void*)data_);
		data_ = 0;
		capacity_ = 0;
		size_ = 0;
	}

	template <typename Type>
	SmallFixedSet<Type>& SmallFixedSet<Type>::operator=(
		const SmallFixedSet& that)
	{
		SmallFixedSet copy(that);
		swap(copy);
		return *this;
	}

	template <typename Type>
	typename SmallFixedSet<Type>::iterator 
		SmallFixedSet<Type>::begin()
	{
		return data_;
	}

	template <typename Type>
	typename SmallFixedSet<Type>::const_iterator 
		SmallFixedSet<Type>::begin() const
	{
		return data_;
	}

	template <typename Type>
	typename SmallFixedSet<Type>::iterator 
		SmallFixedSet<Type>::end()
	{
		return data_ + size_;
	}

	template <typename Type>
	typename SmallFixedSet<Type>::const_iterator 
		SmallFixedSet<Type>::end() const
	{
		return data_ + size_;
	}

	template <typename Type>
	void SmallFixedSet<Type>::swap(
		SmallFixedSet& that)
	{
		std::swap(data_, that.data_);
		std::swap(capacity_, that.capacity_);
		std::swap(size_, that.size_);
	}

	template <typename Type>
	integer SmallFixedSet<Type>::size() const
	{
		return size_;
	}

	template <typename Type>
	integer SmallFixedSet<Type>::capacity() const
	{
		return capacity_;
	}

	template <typename Type>
	bool SmallFixedSet<Type>::empty() const
	{
		return size_ == 0;
	}

	template <typename Type>
	bool SmallFixedSet<Type>::full() const
	{
		return size_ == capacity_;
	}

	template <typename Type>
	Type& SmallFixedSet<Type>::front()
	{
		PENSURE(!empty());
		return *begin();
	}

	template <typename Type>
	const Type& SmallFixedSet<Type>::front() const
	{
		PENSURE(!empty());
		return *begin();
	}

	template <typename Type>
	Type& SmallFixedSet<Type>::back()
	{
		PENSURE(!empty());
		return *(end() - 1);
	}

	template <typename Type>
	const Type& SmallFixedSet<Type>::back() const
	{
		PENSURE(!empty());
		return *(end() - 1);
	}

	template <typename Type>
	typename SmallFixedSet<Type>::iterator 
		SmallFixedSet<Type>::insert(const Type& that)
	{
		if (empty() || back() < that)
		{
			if (full())  
			{
				return end();
			}

			new(end()) Type(that);
			++size_;
			return (end() - 1);
		}

		if (!full())
		{
			new(end()) Type(back());
		}
		back().~Type();

		integer i = size_ - 1;
		for (;i > 0;--i)
		{
			if (data_[i - 1] < that)
			{
				break;
			}
			new(data_ + i) Type(data_[i - 1]);
			data_[i - 1].~Type();
		}

		new(data_ + i) Type(that);
		
		if (!full())
		{
			++size_;
		}

		return data_ + i;
	}

	template <typename Type>
	void SmallFixedSet<Type>::clear()
	{
		StdExt::destruct(begin(), end());
		size_ = 0;
	}

	template <typename Type>
	Type& SmallFixedSet<Type>::operator[](integer index)
	{
		PENSURE2(index >= 0 && index < size_, index, size_);

		return data_[index];
	}

	template <typename Type>
	const Type& SmallFixedSet<Type>::operator[](integer index) const
	{
		PENSURE2(index >= 0 && index < size_, index, size_);

		return data_[index];
	}

}

#endif
