#ifndef PASTEL_NATIVEINTEGER_HPP
#define PASTEL_NATIVEINTEGER_HPP

#include "pastel/sys/nativeinteger.h"

#include <algorithm>

namespace Pastel
{

	template <typename Type>
	NativeInteger<Type>::NativeInteger()
		: data_(0)
	{
	}

	template <typename Type>
	NativeInteger<Type>::NativeInteger(Type that)
		: data_(that)
	{
	}

	template <typename Type>
	void NativeInteger<Type>::swap(NativeInteger& that)
	{
		std::swap(data_, that.data_);
	}

	template <typename Type>
	bool NativeInteger<Type>::operator==(
		const NativeInteger& that) const
	{
		return data_ == that.data_;
	}

	template <typename Type>
	bool NativeInteger<Type>::operator<(
		const NativeInteger& that) const
	{
		return data_ < that.data_;
	}

	template <typename Type>
	Type NativeInteger<Type>::data() const
	{
		return data_;
	}

	template <typename Type>
	bool NativeInteger<Type>::zero() const
	{
		return data_ == 0;
	}

	template <typename Type>
	bool NativeInteger<Type>::negative() const
	{
		return data_ < 0;
	}

	template <typename Type>
	bool NativeInteger<Type>::positive() const
	{
		return data_ > 0;
	}

	template <typename Type>
	NativeInteger<Type> NativeInteger<Type>::operator+() const
	{
		return *this;
	}

	template <typename Type>
	NativeInteger<Type> NativeInteger<Type>::operator-() const
	{
		return NativeInteger<Type>(-data_);
	}

	template <typename Type>
	NativeInteger<Type>& NativeInteger<Type>::operator++()
	{
		++data_;
		return *this;
	}

	template <typename Type>
	NativeInteger<Type>& NativeInteger<Type>::operator--()
	{
		--data_;
		return *this;
	}

	template <typename Type>
	NativeInteger<Type>& NativeInteger<Type>::operator+=(
		const NativeInteger& that)
	{
		data_ += that.data_;
		return *this;
	}

	template <typename Type>
	NativeInteger<Type>& NativeInteger<Type>::operator-=(
		const NativeInteger& that)
	{
		data_ -= that.data_;
		return *this;
	}

	template <typename Type>
	NativeInteger<Type>& NativeInteger<Type>::operator*=(
		const NativeInteger& that)
	{
		data_ *= that.data_;
		return *this;
	}

	template <typename Type>
	NativeInteger<Type>& NativeInteger<Type>::operator/=(
		const NativeInteger& that)
	{
		data_ /= that.data_;
		return *this;
	}

	template <typename Type>
	NativeInteger<Type>& NativeInteger<Type>::operator<<=(
		integer that)
	{
		data_ <<= that;
		return *this;
	}

	template <typename Type>
	NativeInteger<Type>& NativeInteger<Type>::operator>>=(
		integer that)
	{
		data_ >>= that;
		return *this;
	}

	template <typename Type>
	NativeInteger<Type>& NativeInteger<Type>::operator&=(
		const NativeInteger& that)
	{
		data_ &= that.data_;
		return *this;
	}

	template <typename Type>
	NativeInteger<Type>& NativeInteger<Type>::operator|=(
		const NativeInteger& that)
	{
		data_ |= that.data_;
		return *this;
	}

	template <typename Type>
	NativeInteger<Type>& NativeInteger<Type>::operator^=(
		const NativeInteger& that)
	{
		data_ ^= that.data_;
		return *this;
	}

	template <typename Type>
	void swap(NativeInteger<Type>& left, NativeInteger<Type>& right)
	{
		left.swap(right);
	}

	template <typename Type>
	bool zero(const NativeInteger<Type>& that)
	{
		return that.zero();
	}
	
	template <typename Type>
	bool negative(const NativeInteger<Type>& that)
	{
		return that.negative();
	}

	template <typename Type>
	bool positive(const NativeInteger<Type>& that)
	{
		return that.positive();
	}

}

#endif
