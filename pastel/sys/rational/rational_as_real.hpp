#ifndef PASTELSYS_RATIONAL_AS_REAL_HPP
#define PASTELSYS_RATIONAL_AS_REAL_HPP

#include "pastel/sys/rational/rational.h"
#include "pastel/sys/type_traits/is_template_instance.h"

namespace Pastel
{

	// Real

	template <
		typename Type, 
		Requires<IsTemplateInstance<Type, Rational>> = 0>
	bool isNan(const Type& that)
	{
		return that.isNan();
	}

	template <
		typename Type, 
		Requires<IsTemplateInstance<Type, Rational>> = 0>
	bool isInfinity(const Type& that)
	{
		return that.isInfinity();
	}

	template <
		typename Type, 
		Requires<IsTemplateInstance<Type, Rational>> = 0>
	bool isMinusInfinity(const Type& that)
	{
		return that.isMinusInfinity();
	}

	template <typename Integer>
	Rational<Integer> floor(
		const Rational<Integer>& that)
	{
		// Note: n is always >= 0.

		if (zero(that.n()))
		{
			// 0, Nan, -infinity, +infinity.
			return that;
		}

		if (that.n() == 1)
		{
			// Integers (including zero).
			return that.m();
		}

		if (positive(that))
		{
			// Integer-division rounds towards zero,
			// which is floor for positive numbers.
			return that.m() / that.n();
		}
		
		// Integer-division rounds towards zero.
		// Since the number is negative non-integer,
		// integer-division rounds one too high.
		return (that.m() / that.n()) - 1;
	}

	template <typename Integer>
	Rational<Integer> ceil(
		const Rational<Integer>& that)
	{
		// Note: n is always >= 0.

		if (zero(that.n()))
		{
			// 0, Nan, -infinity, +infinity.
			return that;
		}

		if (that.n() == 1)
		{
			// Integers (including zero).
			return that.m();
		}

		if (positive(that))
		{
			// Integer-division rounds towards zero.
			// Since the number is positive non-integer,
			// integer-division rounds one too low.
			return (that.m() / that.n()) + 1;
		}
		
		// Integer-division rounds towards zero,
		// which is ceil for negative numbers.
		return that.m() / that.n();
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
		if (negative(that))
		{
			return -that;
		}

		return that;
	}

	template <typename Integer>
	bool negative(
		const Rational<Integer>& that)
	{
		using Pastel::negative;
		return negative(that.m());
	}

	template <typename Integer>
	bool positive(
		const Rational<Integer>& that)
	{
		using Pastel::positive;
		return positive(that.m());
	}

}

namespace Pastel
{

	// Additive monoid

	template <typename Integer>
	bool zero(
		const Rational<Integer>& that)
	{
		using Pastel::zero;
		return zero(that.m()) && !zero(that.n());
	}

}

namespace Pastel
{

	// Multiplicative monoid

	template <typename Integer>
	Rational<Integer> pow(
		Rational<Integer> that,
		integer p)
	{
		return groupPower(std::move(that), p);
	}

	template <typename Integer>
	bool one(
		const Rational<Integer>& that)
	{
		return that == 1;
	}

}

namespace Pastel
{

	// Printable

	template <typename Integer>
	std::string asString(
		const Rational<Integer>& that)
	{
		return that.asString();
	}

}

#endif
