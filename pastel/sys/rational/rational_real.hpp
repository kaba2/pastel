#ifndef PASTELSYS_RATIONAL_REAL_HPP
#define PASTELSYS_RATIONAL_REAL_HPP

#include "pastel/sys/rational/rational.h"

namespace Pastel
{

	template <typename Integer>
	Rational<Integer>::Rational(real32_ieee that)
		: m_(0)
		, n_(1)
	{
		// The ieee float is a 32-bit number composed as
		// sign (1 bit) biasedExponent (8 bits) mantissa (23 bits)
		// with sign as the most significant bit.

		const uint32 bits = *((const uint32*)&that);
		bool sign = ((bits >> 31) == 1);

		const uint32 exponentBits = (bits >> 23) & 0xFF;
		const uint32 mantissaBits = bits & 0x7FFFFF;

		integer exponent = (integer)exponentBits - 127;

		// Check for zeros (both -0 and +0).

		if ((bits & 0x7FFFFFFF) == 0)
		{
			return;
		}

		// Check for infinities and NaN.
		if (exponentBits == 0xFF)
		{
			if (mantissaBits == 0)
			{
				// Infinities.

				n_ = 0;

				if (sign)
				{
					m_ = -1;
					return;
				}

				m_ = 1;
				return;
			}

			// NaN.

			m_ = 0;
			n_ = 0;
			return;
		}

		// Normal numbers have the value:
		// (-1)^sign * 2^exponent * 1.mantissa

		// Subnormal numbers have the value:
		// (-1)^sign * 2^(exponent + 1) * 0.mantissa

		m_ = mantissaBits;
		n_ <<= 23;

		if (exponentBits != 0)
		{
			// Normal number.
			m_ += n_;
			if (exponent > 0)
			{
				m_ <<= exponent;
			}
			else if (exponent < 0)
			{
				n_ <<= -exponent;
			}
		}
		else
		{
			// Subnormal number.
			if (exponent + 1 > 0)
			{
				m_ <<= (exponent + 1);
			}
			else if (exponent + 1 < 0)
			{
				n_ <<= -(exponent + 1);
			}
		}

		if (sign)
		{
			m_ = -m_;
		}

		simplify();
	}

	template <typename Integer>
	Rational<Integer>::Rational(real64_ieee that)
		: m_(0)
		, n_(1)
	{
		// The ieee double is a 64-bit number composed as
		// sign (1 bit), biased exponent (11 bits), and mantissa (52 bits)
		// with sign as the most significant bit.

		const uint64 bits = *((const uint64*)&that);
		bool sign = ((bits >> 63) == 1);

		const uint64 exponentBits = (bits >> 52) & 0x3FF;
		const uint64 mantissaBits = bits & 0xFFFFFFFFFFFFF;

		integer exponent = (integer)exponentBits - 1023;

		// Check for zeros (both -0 and +0).

		if ((bits & 0x7FFFFFFFFFFFFFFF) == 0)
		{
			return;
		}

		// Check for infinities and NaN.
		if (exponentBits == 0x3FF)
		{
			if (mantissaBits == 0)
			{
				// Infinities.

				n_ = 0;

				if (sign)
				{
					m_ = -1;
					return;
				}

				m_ = 1;
				return;
			}

			// NaN.

			m_ = 0;
			n_ = 0;
			return;
		}

		// Normal numbers have the value:
		// (-1)^sign * 2^exponent * 1.mantissa

		// Subnormal numbers have the value:
		// (-1)^sign * 2^(exponent + 1) * 0.mantissa

		m_ = mantissaBits;
		n_ <<= 52;

		if (exponentBits != 0)
		{
			// Normal number.
			m_ += n_;
			if (exponent > 0)
			{
				m_ <<= exponent;
			}
			else if (exponent < 0)
			{
				n_ <<= -exponent;
			}
		}
		else
		{
			// Subnormal number.
			if (exponent + 1 > 0)
			{
				m_ <<= (exponent + 1);
			}
			else if (exponent + 1 < 0)
			{
				n_ <<= -(exponent + 1);
			}
		}

		if (sign)
		{
			m_ = -m_;
		}

		simplify();
	}

}

#endif
