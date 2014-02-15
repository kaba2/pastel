#ifndef PASTELSYS_MOD_HPP
#define PASTELSYS_MOD_HPP

#include "pastel/sys/mod.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/twos_complement.h"
#include "pastel/sys/bitmask.h"

#include <cmath>

namespace Pastel
{

	template <typename Integer>
	PASTEL_ENABLE_IF(std::is_signed<Integer>, Integer)
		modPowerOfTwo(const Integer& x, integer n)
	{
		PENSURE(!negative(n));

		// This is the portable way to do this;
		// the bit-representation of signed
		// integers is implementation-defined.

		using Unsigned = std::make_unsigned<Integer>::type;

		Unsigned X = signedToTwosComplement(x);
		Unsigned Mask = bitMask<Unsigned>(n);

		if (!negative(x))
		{
			// This is well-defined since X is obtained
			// from an existing signed integer.
			return twosComplementToSigned(X & Mask);
		}

		// Compute mod(|x|, 2^n).
		// Note that, by the C++ standard, -X is the two's 
		// complement for an unsigned integer.
		Unsigned absMod = (-X) & Mask;
		if (zero(absMod))
		{
			return 0;
		}

		// Compute mod(x, n) = 2^n - mod(|x|, 2^n)
		return twosComplementToSigned((Mask - absMod) + 1);
	}

	template <typename Integer>
	PASTEL_ENABLE_IF(std::is_unsigned<Integer>, Integer)
		modPowerOfTwo(const Integer& x, integer n)
	{
		return x & bitMask<Integer>(n);
	}

	template <typename Integer>
	Integer mod(const Integer& x, const Integer& n)
	{
		PENSURE(positive(n));

		if (!negative(x))
		{
			// Since both numbers are non-negative,
			// the % already does what we want.
			return x % n;
		}

		// Compute mod(|x|, n).
		Integer absMod = (-x) % n;
		if (zero(absMod))
		{
			return 0;
		}

		// Compute mod(x, n) = n - mod(|x|, n)
		return n - absMod;
	}

	template <typename Real>
	PASTEL_ENABLE_IF(std::is_floating_point<Real>, Real)
		mod(const Real& x)
	{
		return x - std::floor(x);
	}

	template <typename Real>
	PASTEL_ENABLE_IF(std::is_floating_point<Real>, Real)
		mod(const Real& x, const Real& n)
	{
		PENSURE_OP(n, >, 0);
		return Pastel::mod(x / n) * n;
	}

}

#endif
