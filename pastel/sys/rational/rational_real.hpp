#ifndef PASTELSYS_RATIONAL_REAL_HPP
#define PASTELSYS_RATIONAL_REAL_HPP

#include "pastel/sys/rational/rational.h"
#include "pastel/sys/bit/lowest_bit.h"
#include "pastel/sys/bit/highest_bit.h"
#include "pastel/sys/real/ieee_float.h"
#include "pastel/sys/string/digit.h"

namespace Pastel
{

	template <typename Integer>
	template <
		typename Real,
		Requires<std::is_floating_point<Real>>>
	Real Rational<Integer>::asReal() const
	{
		// Handle the degenerate cases.
		switch(classify())
		{
			case NumberType::Infinity:
				return infinity<Real>();
			case NumberType::MinusInfinity:
				return -infinity<Real>();
			case NumberType::Nan:
				return nan<Real>();
			default:
				// Fall-through
				;
		};

		// Finding the closest floating-point number is
		// tricky, because we do not know the internal
		// representation of the floating-point number.
		// Therefore, we convert the rational number to
		// a string, which is exact up to rounding, and 
		// then use the C++ Standard Library to convert
		// the string to a floating-point number.

		// We choose the number of digits in the string
		// as two digits more than what the precision
		// of the floating-number is. This accounts for
		// the fact that the number of correct digits has 
		// been rounded down.
		enum : integer 
		{
			maxDigits = 
				std::numeric_limits<Real>::digits10 + 2
		};

		return stringAsReal<Real>(
			asString(10, maxDigits));
	}

	template <typename Integer>
	std::string Rational<Integer>::asStringRatio() const
	{
		using Pastel::asString;

		std::string text = asString(m());
		if (n() != 1)
		{
			text += "/" + asString(n());
		}
		return text;
	}

	template <typename Integer>
	std::string Rational<Integer>::asString(
		integer base, 
		integer maxDigits,
		bool showBase) const
	{
		using Pastel::asString;

		ENSURE_OP(base, >=, 2);
		ENSURE_OP(base, >=, 0);

		// Handle the degenerate cases.
		switch(classify())
		{
			case NumberType::Infinity:
				return "inf";
			case NumberType::MinusInfinity:
				return "-inf";
			case NumberType::Nan:
				return "nan";
			default:
				// Fall-through
				;
		};

		// This is where the results is constructed.
		std::string text;
		if (isNegative())
		{
			// Print the minus-sign.
			text += "-";
		}		

		// Reduce to a non-negative number.
		Integer m = abs(m_);

		// Compute the integer-part.
		Integer wholes = m / n();

		// Compute the fractional part.
		m -= wholes * n();
	
		// Print the integer-part.
		text += asString(wholes);

		if (!zero(m) && maxDigits > 0)
		{
			// The number has a fractional part.

			// Print the fractional point.
			text += '.';

			// Print the fractional part.
			for (integer i = 0; i < maxDigits && !zero(m); ++i)
			{
				m *= base;

				integer d = (integer)(m / n());
				m -= d * n();

				text += integerAsDigit(d);
			}
		}

		if (showBase)
		{
			// Print the base.
			text += '_' + asString(base);
		}

		return text;
	}

	template <typename Integer>
	template <
		typename Real,
		Requires<std::is_floating_point<Real>>>
	Rational<Integer>::Rational(
		Real that,
		Integer nMax)
		: m_(0)
		, n_(1)
	{
		ENSURE(!negative(nMax));

		Real logAbs = std::log2(abs(that));
		if (logAbs < -(bits(m()) - 1))
		{
			// The rational number underflows.
			
			// Zero
			m_ = 0;
			n_ = 1;
			return;
		}

		if (logAbs > bits(m()) - 1)
		{
			// The rational number overflows.
			if (negative(that))
			{
				// -Infinity
				m_ = -1;
				n_ = 0;
				return;
			}

			// +Infinity
			m_ = 1;
			n_ = 0;
			return;
		}

		bool nonNegativeThat = (that >= 0);
		if (!nonNegativeThat)
		{
			// Reduce to the non-negative case.
			that = -that;
		}

		// Let n = floor(x).
		Real n = floor(that);

		// What follows is a mediant-binary search over the
		// rational numbers. The search process can be 
		// visualized by the Stern-Brocot tree. 

		// Suppose we start searching for the best mediant
		// from [(0, 1), (1, 0)[. The mediant-binary search will
		// then do the following steps:
		// 
		// 		[(0, 1), (1, 0)[
		//      [(1, 1), (1, 0)[
		//      [(2, 1), (1, 0)[
		//      ...
		//      [(n, 1), (1, 0)[
		//      [(n, 1), (n + 1, 1)[
		//
		// Therefore, we should start straight from 
		//
		// 		[(n, 1), (1, 0)[.
		//
		// We do not start from [(n, 1), (n + 1, 1)[, since
		// then the initialization would need to check whether
		// (n + 1, 1) is better than (n, 1).

		Rational left((integer)n);
		Rational right = infinity<Rational>();
		
		Rational& best = *this;
		best = left;		
		Real minError = that - left.asReal<Real>();

		while(minError > 0)
		{
			// Compute the mediant of left and right.
			Rational mediant(
				left.m() + right.m(),
				left.n() + right.n(),
				SkipSimplify());

			// The simplification can be skipped, since
			// gcd(mediant.m(), mediant.n()) == 1.

			bool mOverflowed = mediant.m() < left.m();
			bool nOverflowed = mediant.n() < left.n();

			if (mOverflowed || 
				nOverflowed ||
				(positive(nMax) && mediant.n() > nMax))
			{
				// Either the numerator or the denominator 
				// overflowed. Since their (non-overflow) gcd = 1, 
				// there cannot be a better approximation than 
				// our current one. Stop the computation.
				break;
			}

			ASSERT(gcd(mediant.m(), mediant.n()) == 1);

			if (that < mediant.asReal<Real>())
			{
				// The mediant is too large. Update the
				// search-interval to end at the mediant.
				right = mediant;
			}
			else
			{
				// The mediant is too small. Update the
				// search-interval to begin from the mediant.
				left = mediant;
			}

			// Compute the error between the rational 
			// approximation and the floating point number.
			Real error = std::abs(mediant.asReal<Real>() - that); 
			if (error < minError) 
			{
				// The error is the smallest thus far. Remember it.
				best = mediant;
				minError = error;
			}
		}

		if (!nonNegativeThat)
		{
			// Negate the number to give the correct sign.
			best.m_ = -best.m_;
		}
    }

}

#endif
