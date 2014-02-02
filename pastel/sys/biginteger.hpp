#ifndef PASTELSYS_BIGINTEGER_HPP
#define PASTELSYS_BIGINTEGER_HPP

#include "pastel/sys/biginteger.h"

namespace Pastel
{

	template <typename BuiltInInteger>
	BigInteger::BigInteger(BuiltInInteger that, 
		PASTEL_ENABLE_IF((std::is_integral<BuiltInInteger>), bool) visible)
		: digits_()
		, sign_(that >= 0)
	{
		construct<BuiltInInteger>(that);
	}

	template <typename BuiltInInteger>
	void BigInteger::construct(
		PASTEL_ENABLE_IF((std::is_signed<BuiltInInteger>), BuiltInInteger) that)
	{
		if (!sign_) {that = -that;}
			
		while(that != 0)
		{
			digits_.push_back(that & 0xFFFF);
			that >>= 16;
		}
	}

	template <typename BuiltInInteger>
	void BigInteger::construct(
		PASTEL_DISABLE_IF((std::is_signed<BuiltInInteger>), BuiltInInteger) that)
	{
		while(that != 0)
		{
			digits_.push_back(that & 0xFFFF);
			that >>= 16;
		}
	}

}

namespace Pastel
{

	inline void swap(BigInteger& left, BigInteger& right)
	{
		left.swap(right);
	}

	// Real (partial)

	template <>
	class Real_Function<BigInteger>
	{
	public:
		static const bool Exists = true;

		// infinity() and nan() deliberately
		// not implemented because there are no
		// corresponding element.
	};

	inline const BigInteger& floor(const BigInteger& that)
	{
		return that;
	}

	inline const BigInteger& ceil(const BigInteger& that)
	{
		return that;
	}

	// Ordered additive monoid

	inline BigInteger abs(const BigInteger& that)
	{
		return that.negative() ? -that : that;
	}

	inline bool negative(const BigInteger& that)
	{
		return that.negative();
	}

	inline bool positive(const BigInteger& that)
	{
		return that.positive();
	}

	// Additive monoid

	inline bool zero(const BigInteger& that)
	{
		return that.zero();
	}

}

#endif
