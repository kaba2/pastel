#ifndef PASTEL_RATIONAL_HPP
#define PASTEL_RATIONAL_HPP

#include "pastel/sys/rational.h"

#include "pastel/sys/integer_tools.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Integer>
	Rational<Integer>::Rational()
		: numerator_(0)
		, denominator_(1)
	{
	}

	template <typename Integer>
	Rational<Integer>::Rational(
		const Rational& that)
		: numerator_(that.numerator_)
		, denominator_(that.denominator_)
	{
	}

	template <typename Integer>
	Rational<Integer>::Rational(
		Rational&& that)
		: numerator_(std::move(that.numerator_))
		, denominator_(std::move(that.denominator_))
	{
	}

	template <typename Integer>
	template <typename That_Integer>
	Rational<Integer>::Rational(
		That_Integer wholes, 
		PASTEL_ENABLE_IF(IsNativeOrInteger<That_Integer>, Private)*)
		: numerator_(wholes)
		, denominator_(1)
	{
	}

	template <typename Integer>
	template <
		typename Numerator_Integer, 
		typename Denominator_Integer>
	Rational<Integer>::Rational(
		Numerator_Integer numerator,
		Denominator_Integer denominator,
		PASTEL_ENABLE_IF((AreNativeOrInteger<Numerator_Integer, Denominator_Integer>), Private)*)
		: numerator_(std::move(numerator))
		, denominator_(std::move(denominator))
	{
		simplify();
	}

	template <typename Integer>
	Rational<Integer>::Rational(real32_ieee that)
		: numerator_(0)
		, denominator_(1)
	{
		// The ieee float is a 32-bit number composed as
		// sign (1 bit) biasedExponent (8 bits) mantissa (23 bits)
		// with sign as the most significant bit.

		const uint32 bits = *((const uint32*)&that);
		const bool sign = ((bits >> 31) == 1);
		const uint32 exponentBits = (bits >> 23) & 0xFF;
		const uint32 mantissaBits = bits & 0x7FFFFF;

		const integer exponent = (integer)exponentBits - 127;

		// Check for zeros (both -0 and +0).
		if ((bits & 0x7FFFFFFF) == 0)
		{
			return;
		}

		// Check for infinities and NaN.
		if (exponentBits == 0xFF)
		{
			if (mantissaBits == 0)
			{
				// Infinities.

				denominator_ = 0;

				if (sign)
				{
					numerator_ = -1;
					return;
				}

				numerator_ = 1;
				return;
			}

			// NaN.

			numerator_ = 0;
			denominator_ = 0;
			return;
		}

		// Normal numbers have the value:
		// (-1)^sign * 2^exponent * 1.mantissa

		// Subnormal numbers have the value:
		// (-1)^sign * 2^(exponent + 1) * 0.mantissa

		numerator_ = mantissaBits;
		denominator_ <<= 23;

		if (exponentBits != 0)
		{
			// Normal number.
			numerator_ += denominator_;
			if (exponent > 0)
			{
				numerator_ <<= exponent;
			}
			else if (exponent < 0)
			{
				denominator_ <<= -exponent;
			}
		}
		else
		{
			// Subnormal number.
			if (exponent + 1 > 0)
			{
				numerator_ <<= (exponent + 1);
			}
			else if (exponent + 1 < 0)
			{
				denominator_ <<= -(exponent + 1);
			}
		}

		if (sign)
		{
			numerator_ = -numerator_;
		}

		simplify();
	}

	template <typename Integer>
	Rational<Integer>::Rational(real64_ieee that)
		: numerator_(0)
		, denominator_(1)
	{
		// The ieee double is a 64-bit number composed as
		// sign (1 bit), biased exponent (11 bits), and mantissa (52 bits)
		// with sign as the most significant bit.

		const uint64 bits = *((const uint64*)&that);
		const bool sign = ((bits >> 63) == 1);
		const uint64 exponentBits = (bits >> 52) & 0x3FF;
		const uint64 mantissaBits = bits & 0xFFFFFFFFFFFFF;

		const integer exponent = (integer)exponentBits - 1023;

		// Check for zeros (both -0 and +0).
		if ((bits & 0x7FFFFFFFFFFFFFFF) == 0)
		{
			return;
		}

		// Check for infinities and NaN.
		if (exponentBits == 0x3FF)
		{
			if (mantissaBits == 0)
			{
				// Infinities.

				denominator_ = 0;

				if (sign)
				{
					numerator_ = -1;
					return;
				}

				numerator_ = 1;
				return;
			}

			// NaN.

			numerator_ = 0;
			denominator_ = 0;
			return;
		}

		// Normal numbers have the value:
		// (-1)^sign * 2^exponent * 1.mantissa

		// Subnormal numbers have the value:
		// (-1)^sign * 2^(exponent + 1) * 0.mantissa

		numerator_ = mantissaBits;
		denominator_ <<= 52;

		if (exponentBits != 0)
		{
			// Normal number.
			numerator_ += denominator_;
			if (exponent > 0)
			{
				numerator_ <<= exponent;
			}
			else if (exponent < 0)
			{
				denominator_ <<= -exponent;
			}
		}
		else
		{
			// Subnormal number.
			if (exponent + 1 > 0)
			{
				numerator_ <<= (exponent + 1);
			}
			else if (exponent + 1 < 0)
			{
				denominator_ <<= -(exponent + 1);
			}
		}

		if (sign)
		{
			numerator_ = -numerator_;
		}

		simplify();
	}

	template <typename Integer>
	Rational<Integer>& Rational<Integer>::operator=(
		Rational that)
	{
		swap(that);

		return *this;
	}

	template <typename Integer>
	void Rational<Integer>::swap(
		Rational& that)
	{
		using std::swap;

		swap(numerator_, that.numerator_);
		swap(denominator_, that.denominator_);
	}

	template <typename Integer>
	void Rational<Integer>::set(Integer numerator, Integer denominator)
	{
		Rational copy(
			std::move(numerator), 
			std::move(denominator));

		swap(copy);
	}

	template <typename Integer>
	const Integer& Rational<Integer>::numerator() const
	{
		return numerator_;
	}

	template <typename Integer>
	const Integer& Rational<Integer>::denominator() const
	{
		return denominator_;
	}

	template <typename Integer>
	Rational<Integer>& Rational<Integer>::operator+=(
		Rational that)
	{
		/*
		+	|	Num	NaN	+oo	-oo	+0 -0
		---+-------------------------
		Num|	Num
		NaN|	NaN	NaN
		+oo|	+oo	NaN	+oo
		-oo|	-oo	NaN	NaN	-oo
		+0 |	Num	NaN	+oo	-oo	+0
		-0 |    Num NaN +oo -oo
		*/

		typename NumberType::Enum thisType = classify();
		typename NumberType::Enum thatType = that.classify();

		Rational result;

		if (thisType == NumberType::Nan ||
			thatType == NumberType::Nan ||
			(thisType == NumberType::Infinity &&
			thatType == NumberType::MinusInfinity) ||
			(thisType == NumberType::MinusInfinity &&
			thatType == NumberType::Infinity))
		{
			// The result is NaN.
			result.set(0, 0, SkipSimplify());
		}
		else if (
			thisType == NumberType::Infinity ||
			thatType == NumberType::Infinity)
		{
			// The result is +oo.
			result.set(1, 0, SkipSimplify());
		}
		else if (
			thisType == NumberType::MinusInfinity ||
			thatType == NumberType::MinusInfinity)
		{
			// The result is -oo.
			result.set(-1, 0, SkipSimplify());
		}
		else
		{
			// Both are just normal numbers.
			result.set(
				numerator_ * that.denominator_ +
				std::move(that.numerator_) * denominator_,
				denominator_ * that.denominator_);
		}

		swap(result);

		return *this;
	}

	template <typename Integer>
	Rational<Integer>& Rational<Integer>::operator-=(
		Rational that)
	{
		return (*this) +=
			Rational(
			-that.numerator_, std::move(that.denominator_));
	}

	template <typename Integer>
	Rational<Integer>& Rational<Integer>::operator*=(
		Rational that)
	{
		/*
		*	|	Num	NaN	+oo	-oo	0
		---+---------------------------
		Num|	Num
		NaN|	NaN	NaN
		+oo|	+oo	NaN	+oo
		-oo|	-oo	NaN	-oo	+oo
		0  |      0 NaN	0	0   0
		*/

		typename NumberType::Enum thisType = classify();
		typename NumberType::Enum thatType = that.classify();

		Rational result;

		if (thisType == NumberType::Nan ||
			thatType == NumberType::Nan)
		{
			// The result is NaN.
			result.set(0, 0, SkipSimplify());
		}
		else if ((thisType == NumberType::Infinity &&
			thatType == NumberType::Infinity) ||
			(thisType == NumberType::MinusInfinity &&
			thatType == NumberType::MinusInfinity))
		{
			// The result is +oo.
			result.set(1, 0, SkipSimplify());
		}
		else if ((thisType == NumberType::Infinity &&
			thatType == NumberType::MinusInfinity) ||
			(thisType == NumberType::MinusInfinity &&
			thatType == NumberType::Infinity))
		{
			// The result is -oo.
			result.set(-1, 0, SkipSimplify());
		}
		else if (thisType == NumberType::Zero ||
			thatType == NumberType::Zero)
		{
			// The result is zero.
			result.set(0, 1, SkipSimplify());
		}
		else
		{
			// Both are just normal numbers.
			result.set(
				numerator_  * std::move(that.numerator_),
				denominator_ * std::move(that.denominator_));
		}

		swap(result);

		return *this;
	}

	template <typename Integer>
	Rational<Integer>& Rational<Integer>::operator/=(
		Rational that)
	{
		return (*this) *=
			Rational(
			std::move(that.denominator_), 
			std::move(that.numerator_));
	}

	template <typename Integer>
	Rational<Integer> Rational<Integer>::operator-() const
	{
		return Rational(-numerator_, denominator_);
	}

	template <typename Integer>
	Rational<Integer> Rational<Integer>::operator+() const
	{
		return *this;
	}

	// Private

	template <typename Integer>
	Rational<Integer>::Rational(
		Integer numerator, Integer denominator,
		SkipSimplify)
		: numerator_(std::move(numerator))
		, denominator_(std::move(denominator))
	{
	}

	template <typename Integer>
	void Rational<Integer>::set(
		Integer numerator, Integer denominator,
		SkipSimplify)
	{
		Rational copy(
			std::move(numerator), 
			std::move(denominator),
			SkipSimplify());

		swap(copy);
	}

	template <typename Integer>
	void Rational<Integer>::simplify()
	{
		if (!(zero(numerator_) &&
			zero(denominator_)))
		{
			// The number is not NaN, so
			// divide by the GCD(numerator, denominator)

			Integer theGcd(gcd(numerator_, denominator_));

			numerator_ /= theGcd;
			denominator_ /= std::move(theGcd);

			if (negative(denominator_))
			{
				numerator_ = -numerator_;
				denominator_ = -denominator_;
			}
		}
	}

	template <typename Integer>
	typename Rational<Integer>::NumberType::Enum
		Rational<Integer>::classify() const
	{
		typename NumberType::Enum result = NumberType::Normal;
		if (!zero(denominator_))
		{
			if (zero(numerator_))
			{
				result = NumberType::Zero;
			}
		}
		else
		{
			if (zero(numerator_))
			{
				result = NumberType::Nan;
			}
			else
			{
				if (positive(numerator_))
				{
					result = NumberType::Infinity;
				}
				else
				{
					result = NumberType::MinusInfinity;
				}
			}
		}

		return result;
	}

	template <typename Integer>
	bool Rational<Integer>::equal(const Rational& that) const
	{
		// Because GCD(numerator, denominator) = 1,
		// the rationals are unique.

		// For NaN's, we have decided NaN == NaN is true.

		return numerator_ == that.numerator_ &&
			denominator_ == that.denominator_;
	}

	template <typename Integer>
	bool Rational<Integer>::lessThan(const Rational& that) const
	{
		// Let the numbers be:
		// (a / b) and (c / d)
		// and define:
		// det = ad - bc

		// By invariant:
		// b, d >= 0

		// It does not hold that:
		// oo < oo
		// -oo < -oo
		// NaN < x
		// x < NaN

		// zero(b)	| zero(d)	| result
		// ---------+-----------+----------------
		// 0		| 0			| det < 0
		// 0		| 1			| c > 0
		// 1		| 0			| a < 0
		// 1		| 1			| c > 0 && a < 0

		const Integer& a = numerator_;
		const Integer& b = denominator_;
		const Integer& c = that.numerator_;
		const Integer& d = that.denominator_;

		bool result = false;

		if (zero(b))
		{
			if (zero(d))
			{
				result = positive(c) && negative(a);
			}
			else
			{
				result = negative(a);
			}
		}
		else
		{
			if (zero(d))
			{
				result = positive(c);
			}
			else
			{
				Integer det(a * d - b * c);
				result = negative(det);
			}
		}

		return result;
	}

}

#endif
