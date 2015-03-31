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
		EnableIf<std::is_floating_point<Real>>>
	Real Rational<Integer>::asReal() const
	{
		return stringAsReal<Real>(asString(10, 16));
	}

	template <typename Integer>
	std::string Rational<Integer>::asStringRatio() const
	{
		std::string text = Pastel::asString(m());
		if (n() != 1)
		{
			text += "/" + Pastel::asString(n());
		}
		return text;
	}

	template <typename Integer>
	std::string Rational<Integer>::asString(
		integer base, 
		integer maxDigits,
		bool showBase) const
	{
		// Handle the degenerate cases.
		switch(classify())
		{
			case NumberType::Infinity:
				return "inf";
			case NumberType::MinusInfinity:
				return "-inf";
			case NumberType::Nan:
				return "nan";
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
		text += Pastel::asString(wholes);

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
			text += '_' + Pastel::asString(base);
		}

		return text;
	}

	template <typename Integer>
	template <
		typename Real,
		EnableIf<std::is_floating_point<Real>>>
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
			best.m_ = -best.m_;
		}
    }

	template <typename Integer>
	template <
		integer E, 
		integer M,
		typename Float_Ieee>
	void Rational<Integer>::setIeee(Float_Ieee that)
	{
		integer B = bits(m_);
		static PASTEL_CONSTEXPR integer Bits = CHAR_BIT * sizeof(Float_Ieee);
		PASTEL_STATIC_ASSERT(E > 0);
		PASTEL_STATIC_ASSERT(M > 0);
		PASTEL_STATIC_ASSERT(1 + E + M == Bits);

		static PASTEL_CONSTEXPR integer Bias = (1 << (E - 1)) - 1;
		
		using Word = Uint<Bits>;
		PASTEL_STATIC_ASSERT(sizeof(Word) == sizeof(Float_Ieee));

		// The ieee float is an n-bit number composed as
		// * sign (1 bit) 
		// * biased exponent (E bits)
		// * mantissa (M bits)
		// with sign as the most significant bit.

		Word bits = *((const Word*)&that);

		if ((bits & bitMask<Word>(0, Bits - 1)) == 0)
		{
			// Check for zeros (both -0 and +0).
			return;
		}

		// Extract the sign-bit.
		bool sign = (((bits >> (Bits - 1)) & 1) == 1);

		// Extract the exponent-bits.
		Word exponentBits = (bits >> M) & bitMask<Word>(0, E);

		// Extract the mantissa-bits.
		Word mantissaBits = bits & bitMask<Word>(0, M);

		// Compute the actual exponent by removing the bias.
		integer exponent = (integer)exponentBits - Bias;

		// Check for infinities and NaN.
		if (exponentBits == bitMask<Word>(0, E))
		{
			if (mantissaBits == 0)
			{
				// Infinities.
				n_ = 0;

				if (sign)
				{
					// -infinity
					m_ = -1;
					return;
				}

				// +Infinity
				m_ = 1;
				return;
			}

			// Not-a-number (NaN)
			m_ = 0;
			n_ = 0;
			return;
		}

		integer mantissaShift = 
			lowestBit(mantissaBits);
		ASSERT_OP(mantissaShift, >=, 0);

		integer MB = M - mantissaShift;
		mantissaBits >>= mantissaShift;

		bool isNormal = (exponentBits > 0);
		if (isNormal)
		{
			// Normal number.

			// Normal numbers have the value:
			// (-1)^sign * 2^exponent * (1 + mantissaBits / 2^M)
			// = (-1)^sign * 2^(exponent - M) * (2^M + mantissaBits)

			if (exponent >= B - 1)
			{
				// Overflow

				if (sign)
				{
					// -Infinity
					m_ = -1;
					return;
				}

				// +Infinity
				m_ = 1;
				return;
			}

			if (exponent < -B)
			{
				// Underflow

				// Zero
				return;
			}

			if (exponent >= MB)
			{
				// Exact integer
				m_ = mantissaBits;
				m_ <<= exponent - MB;
				if (sign)
				{
					m_ = -m_;
				}
				return;
			}

			integer shift = std::max(MB - exponent - (B - 1), 0);
			mantissaBits >>= shift;
			MB -= shift;

			// Exact rational, when shift == 0.
			// Rounded rational, otherwise.

			m_ = 1;
			m_ <<= MB;
			m_ += mantissaBits;
			n_ <<= MB - exponent;
		}
		else
		{
			// Subnormal number.

			// Subnormal numbers have the value:
			// (-1)^sign * 2^(exponent + 1) * 0.mantissa

			if (exponent >= B - 1)
			{
				// Overflow

				if (sign)
				{
					// -Infinity
					m_ = -1;
					return;
				}

				// +Infinity
				m_ = 1;
				return;
			}

			if (exponent < -B ||
				(exponent == -(B - 1) &&
				mantissaBits < (1 << (MB - 1))))
			{
				// Underflow

				// Zero
				return;
			}

			if (exponent >= MB)
			{
				// Exact integer
				m_ = mantissaBits;
				m_ <<= exponent - MB;
				if (sign)
				{
					m_ = -m_;
				}
				return;
			}

			integer shift = std::max(MB - exponent - (B - 1), 0);
			mantissaBits >>= shift;
			MB -= shift;

			// Exact rational, when shift == 0.
			// Rounded rational, otherwise.

			m_ = 1;
			m_ <<= MB;
			m_ += mantissaBits;
			n_ <<= MB - exponent;
		}

		if (sign)
		{
			m_ = -m_;
		}

		simplify();
	}

}

#endif
