#ifndef PASTEL_RATIONAL_MORE_HPP
#define PASTEL_RATIONAL_MORE_HPP

#include "pastel/sys/rational_more.h"

namespace Pastel
{

	template <typename Integer>
	Rational<Integer> inverse(const Rational<Integer>& x)
	{
		return Rational<Integer>(
			x.denominator(),
			x.numerator());
	}

	template <typename Integer>
	Integer floor(const Rational<Integer>& x)
	{
		// Note: Denominator is always >= 0.
		if (x.denominator() == 1)
		{
			return x.numerator();
		}

		return x.numerator() / x.denominator();
	}

	template <typename Integer>
	Integer ceil(const Rational<Integer>& x)
	{
		// Note: Denominator is always >= 0.
		if (x.denominator() == 1)
		{
			return x.numerator();
		}

		Integer result = floor(x);
		if (result * x.denominator() != x.numerator())
		{
			++result;
		}

		return result;
	}

	template <typename Integer>
	class Infinity<Rational<Integer>>
	{
	public:
		Rational<Integer> operator()() const
		{
			return Rational<Integer>(1, 0);
		}
	};

	template <typename Integer>
	class Nan<Rational<Integer>>
	{
	public:
		Rational<Integer> operator()() const
		{
			return Rational<Integer>(0, 0);
		}
	};

	template <typename Integer>
	bool zero(const Rational<Integer>& that)
	{
		return Pastel::zero(that.numerator());
	}

	template <typename Integer>
	bool negative(
		const Rational<Integer>& that)
	{
		return Pastel::negative(that.numerator());
	}

	template <typename Integer>
	bool positive(const Rational<Integer>& that)
	{
		return Pastel::positive(that.numerator());
	}

	template <typename Real, typename Integer>
	Real toReal(const Rational<Integer>& that)
	{
		return (Real)that.numerator() / (Real)that.denominator();
	}

	template <typename Integer>
	Rational<Integer> fraction(const Rational<Integer>& x)
	{
		if (!positive(x) && x < 1)
		{
			return x;
		}

		return x - floor(x);
	}

	template <typename Integer>
	Rational<Integer> remainder(
		const Rational<Integer>& x,
		const Rational<Integer>& divider)
	{
		if (!positive(x) && x < divider)
		{
			return x;
		}

		return x - floor(x / divider) * divider;
	}

	template <typename Integer>
	Rational<Integer> multiplyByPowerOf2(
		const Rational<Integer>& x,
		integer power)
	{
		ENSURE_OP(power, >=, 0);
		if (power == 0)
		{
			return x;
		}

		return Rational<Integer>(
			x.numerator() << power,
			x.denominator());
	}

	template <typename Integer>
	Rational<Integer> divideByPowerOf2(
		const Rational<Integer>& x,
		integer power)
	{
		ENSURE_OP(power, >=, 0);
		if (power == 0)
		{
			return x;
		}

		return Rational<Integer>(
			x.numerator(), x.denominator() << power);
	}

	template <typename Integer>
	Rational<Integer> mabs(const Rational<Integer>& that)
	{
		if (negative(that))
		{
			return -that;
		}

		return that;
	}

}

#endif
