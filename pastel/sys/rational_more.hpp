#ifndef PASTEL_RATIONAL_MORE_HPP
#define PASTEL_RATIONAL_MORE_HPP

#include "pastel/sys/rational_more.h"

namespace Pastel
{

	template <typename Integer>
	class Real_Function<Rational<Integer> >
	{
	public:
		static const bool Exists = true;

		Rational<Integer> infinity()
		{
			return Rational<Integer>(1, 0);
		}

		Rational<Integer> nan()
		{
			return Rational<Integer>(0, 0);
		}

		Rational<Integer> inverse(
			const Rational<Integer>& that)
		{
			return Rational<Integer>(
				that.denominator(),
				that.numerator());
		}

		Rational<Integer> floor(
			const Rational<Integer>& that)
		{
			// Note: Denominator is always >= 0.
			if (that.denominator() == 1)
			{
				return that.numerator();
			}

			return that.numerator() / that.denominator();
		}

		Rational<Integer> ceil(
			const Rational<Integer>& that)
		{
			// Note: Denominator is always >= 0.
			if (that.denominator() == 1)
			{
				return that.numerator();
			}

			Integer result = floor(that);
			if (result * that.denominator() != that.numerator())
			{
				++result;
			}

			return result;
		}

		bool zero(
			const Rational<Integer>& that)
		{
			return Pastel::zero(that.numerator());
		}

		bool negative(
			const Rational<Integer>& that)
		{
			return Pastel::negative(that.numerator());
		}

		bool positive(
			const Rational<Integer>& that)
		{
			return Pastel::positive(that.numerator());
		}
	};

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
	Rational<Integer> multiplyByPowerOf2(
		const Rational<Integer>& that,
		integer power)
	{
		ENSURE_OP(power, >=, 0);
		if (power == 0)
		{
			return that;
		}

		return Rational<Integer>(
			that.numerator() << power,
			that.denominator());
	}

	template <typename Integer>
	Rational<Integer> divideByPowerOf2(
		const Rational<Integer>& that,
		integer power)
	{
		ENSURE_OP(power, >=, 0);
		if (power == 0)
		{
			return that;
		}

		return Rational<Integer>(
			that.numerator(), that.denominator() << power);
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
