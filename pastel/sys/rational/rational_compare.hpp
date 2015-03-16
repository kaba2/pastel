#ifndef PASTELSYS_RATIONAL_COMPARE_HPP
#define PASTELSYS_RATIONAL_COMPARE_HPP

#include "pastel/sys/rational/rational.h"

namespace Pastel
{

	template <typename Integer>
	bool Rational<Integer>::equal(const Rational& that) const
	{
		// Because GCD(m, n) = 1,
		// the rationals are unique.

		// For NaN's, we have decided NaN == NaN is true.

		return m_ == that.m_ &&
			n_ == that.n_;
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

		const Integer& a = m_;
		const Integer& b = n_;
		const Integer& c = that.m_;
		const Integer& d = that.n_;

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
