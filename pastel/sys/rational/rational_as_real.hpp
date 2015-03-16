#ifndef PASTELSYS_RATIONAL_AS_REAL_HPP
#define PASTELSYS_RATIONAL_AS_REAL_HPP

#include "pastel/sys/rational/rational.h"

namespace Pastel
{

	template <typename Type>
	struct IsRational
	{
	};

	template <typename Integer>
	struct IsRational<Rational<Integer>>
	{
		static PASTEL_CONSTEXPR bool value = true;
	};

}

namespace Pastel
{

	// Real

	template <typename Type, EnableIf<IsRational<Type>> = 0>
	Type infinity()
	{
		return Type(1, 0);
	}

	template <typename Type, EnableIf<IsRational<Type>> = 0>
	Type nan()
	{
		return Type(0, 0);
	}

	template <typename Type, EnableIf<IsRational<Type>> = 0>
	bool isNan(const Type& that)
	{
		return that.isNan();
	}

	template <typename Type, EnableIf<IsRational<Type>> = 0>
	bool isInfinity(const Type& that)
	{
		return that.isInfinity();
	}

	template <typename Integer>
	Integer floor(
		const Rational<Integer>& that)
	{
		// Note: n is always >= 0.
		if (that.n() == 1)
		{
			return that.m();
		}

		return that.m() / that.n();
	}

	template <typename Integer>
	Integer ceil(
		const Rational<Integer>& that)
	{
		// Note: n is always >= 0.
		if (that.n() == 1)
		{
			return that.m();
		}

		Integer result = floor(that);
		if (result * that.n() != that.m())
		{
			++result;
		}

		return result;
	}

}

namespace Pastel
{

	// Field

	template <typename Integer>
	Rational<Integer> inverse(
		const Rational<Integer>& that)
	{
		return Rational<Integer>(
			that.n(),
			that.m());
	}

}

namespace Pastel
{

	// Ordered additive monoid

	template <typename Integer>
	Rational<Integer> abs(
		const Rational<Integer>& that)
	{
		return mabs(that);
	}

	template <typename Integer>
	bool negative(
		const Rational<Integer>& that)
	{
		return Pastel::negative(that.m());
	}

	template <typename Integer>
	bool positive(
		const Rational<Integer>& that)
	{
		return Pastel::positive(that.m());
	}

}

namespace Pastel
{

	// Additive monoid

	template <typename Integer>
	bool zero(
		const Rational<Integer>& that)
	{
		return Pastel::zero(that.m()) && !Pastel::zero(that.n());
	}

}

namespace Pastel
{

	template <typename Integer>
	Rational<Integer> fraction(
		const Rational<Integer>& that)
	{
		if (!positive(that) && that < 1)
		{
			return that;
		}

		return that - floor(that);
	}

	template <typename Integer>
	Rational<Integer> remainder(
		const Rational<Integer>& that,
		const Rational<Integer>& divider)
	{
		if (!positive(that) && that < divider)
		{
			return that;
		}

		return that - floor(that / divider) * divider;
	}

	template <typename Integer>
	Rational<Integer> multiplyByPowerOfTwo(
		const Rational<Integer>& that,
		integer power)
	{
		ENSURE_OP(power, >=, 0);
		if (power == 0)
		{
			return that;
		}

		return Rational<Integer>(
			that.m() << power,
			that.n());
	}

	template <typename Integer>
	Rational<Integer> divideByPowerOfTwo(
		const Rational<Integer>& that,
		integer power)
	{
		ENSURE_OP(power, >=, 0);
		if (power == 0)
		{
			return that;
		}

		return Rational<Integer>(
			that.m(), that.n() << power);
	}

	template <typename Integer>
	Rational<Integer> mabs(
		const Rational<Integer>& that)
	{
		if (negative(that))
		{
			return -that;
		}

		return that;
	}

}

#endif
