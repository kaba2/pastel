#ifndef PASTELSYS_RATIONAL_AS_REAL_HPP
#define PASTELSYS_RATIONAL_AS_REAL_HPP

#include "pastel/sys/rational/rational.h"
#include "pastel/sys/type_traits/is_template_instance.h"

namespace Pastel
{

	// Real

	template <
		typename Type, 
		EnableIf<IsTemplateInstance<Type, Rational>> = 0>
	Type infinity()
	{
		return Type(1, 0);
	}

	template <
		typename Type, 
		EnableIf<IsTemplateInstance<Type, Rational>> = 0>
	Type nan()
	{
		return Type(0, 0);
	}

	template <
		typename Type, 
		EnableIf<IsTemplateInstance<Type, Rational>> = 0>
	bool isNan(const Type& that)
	{
		return that.isNan();
	}

	template <
		typename Type, 
		EnableIf<IsTemplateInstance<Type, Rational>> = 0>
	bool isInfinity(const Type& that)
	{
		return that.isInfinity();
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

#endif
