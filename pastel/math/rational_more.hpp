#ifndef PASTELMATH_RATIONAL_MORE_HPP
#define PASTELMATH_RATIONAL_MORE_HPP

#include "pastel/math/rational_more.h"

namespace Pastel
{

	template <typename Integer>
	Rational<Integer> infinity(Rational<Integer>*)
	{
		return Rational<Integer>(1, 0);
	}

	template <typename Integer>
	Rational<Integer> nan(Rational<Integer>*)
	{
		return Rational<Integer>(0, 0);
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
	Integer floor(const Rational<Integer>& x)
	{
		if (x.denominator() == 1)
		{
			return x.numerator();
		}

		return x.numerator() / x.denominator();
	}

	// Optimization functions

	template <typename Integer>
	Rational<Integer> multiplyByPowerOf2(
		const Rational<Integer>& x,
		integer power)
	{
		ENSURE1(power >= 0, power);
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
		ENSURE1(power >= 0, power);
		if (power == 0)
		{
			return x;
		}

		return Rational<Integer>(
			x.numerator(), x.denominator() << power);
	}

	template <typename Integer>
	Rational<Integer> inverse(const Rational<Integer>& x)
	{
		return Rational<Integer>(
			x.denominator(),
			x.numerator());
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

	template <typename Integer>
	bool zero(const Rational<Integer>& that)
	{
		return Pastel::zero(that.numerator());
	}


	template <typename Integer>
	float toFloat(const Rational<Integer>& that)
	{
		return 0;
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
