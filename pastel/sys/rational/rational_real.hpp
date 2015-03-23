#ifndef PASTELSYS_RATIONAL_REAL_HPP
#define PASTELSYS_RATIONAL_REAL_HPP

#include "pastel/sys/rational/rational.h"
#include "pastel/sys/bit/lowest_bit.h"
#include "pastel/sys/bit/highest_bit.h"
#include "pastel/sys/real/ieee_float.h"

namespace Pastel
{

	template <typename Integer>
	template <
		typename Real,
		EnableIf<std::is_floating_point<Real>>>
	Real Rational<Integer>::asReal() const
	{
		// The binary expansion of a rational number
		// can be computed by using long-division.

		/*
		switch(classify())
		{
			case NumberType::Infinity:
				return infinity<Real>();
			case NumberType::MinusInfinity:
				return -infinity<Real>();
			case NumberType::Nan:
				return nan<Real>();
			case NumberType::Zero:
				return 0;
		};

		Integer m = abs(m_);
		Integer n = n_;

		Integer wholes = m / n;
		m -= wholes * n;

		integer low = lowestBit(wholes);
		integer high = highestBit(wholes) + 1;
		integer wholesBits = high - low;

		wholes >>= low;

		if (wholesBits > 64)
		{
			integer truncatedBits = (wholesBits - 64);
			wholes >>= truncatedBits;

			return asIeeeFloat<Real>({ negative(*this), truncatedBits, (uint64)wholes });
		}
		integer exponent = low;
		high -= low;
		low = 0;

		// The highest bit is pushed over;
		// it is implicitly assumed to be 1.
		integer shift = 64 - high;
		exponent += shift;
		
		Integer mantissa = (wholes << shift);

		//integer nBits = highestBit(n) + 1;
		*/

		return 0;
	}

	template <typename Integer>
	template <
		typename Real,
		EnableIf<std::is_floating_point<Real>>>
	Rational<Integer>::Rational(Real that)
		: m_(0)
		, n_(1)
	{
		/*
		// The ieee float is a 32-bit number composed as
		// * 1-bit sign (1 bit), 
		// * 8-bit biased exponent, 
		// * 23-bit mantissa.

		// The ieee float is a 64-bit number composed as
		// * 1-bit sign (1 bit), 
		// * 11-bit biased exponent, 
		// * 52-bit mantissa.

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

		// Let n = floor(x).
		Real n = floor(that);

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

		// FIX: generalize for a wider range of values
		// by extracting the integer floor(that) directly 
		// from its IEEE-presentation.

		Rational left((integer)n);
		Rational right(1, 0, SkipSimplify());
		
		Rational& best = *this;
		best = left;		
		Real minError = that - left.asReal<Real>();

		Real epsilon = 1E-15;
		while(minError > epsilon) 
		{
			// Compute the mediant of left and right.
			Rational mediant(
				left.m() + right.m(),
				left.n() + right.n(),
				SkipSimplify());

			if (mediant.m() < left.m() ||
				mediant.n() < left.n())
			{
				// Either the numerator or the denominator 
				// overflowed. Since their (non-overflow) gcd = 1, 
				// there cannot be a better approximation than 
				// our current one. Stop the computation.
				break;
			}

			//ASSERT(gcd(mediant.m(), mediant.n()) == 1);

			if (that < mediant.asReal<Real>())
			{
				right = mediant;
			}
			else
			{
				left = mediant;
			}

			Real error = std::abs(mediant.asReal<Real>() - that); 
			if (error < minError) 
			{
				best = mediant;
				minError = error;
			}
		}
		*/
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
