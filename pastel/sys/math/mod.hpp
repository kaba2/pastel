#ifndef PASTELSYS_MOD_HPP
#define PASTELSYS_MOD_HPP

#include "pastel/sys/math/mod.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/bit/twos_complement.h"
#include "pastel/sys/bit/bitmask.h"
#include "pastel/sys/math/divide_infinity.h"

#include <cmath>

namespace Pastel
{

	template <typename Integer, Requires<std::is_signed<Integer>>>
	Integer modPowerOfTwo(const Integer& x, integer n)
	{
		PENSURE(!negative(n));

		// This is the portable way to do this;
		// the bit-representation of signed
		// integers is implementation-defined.

		using Unsigned = typename std::make_unsigned<Integer>::type;

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

	template <typename Integer, Requires<std::is_unsigned<Integer>>>
	Integer modPowerOfTwo(const Integer& x, integer n)
	{
		return x & bitMask<Integer>(n);
	}

	template <
		Integer_Concept_ Integer,
		Integer_Concept_ N_Integer
	>
	Integer mod(const Integer& x, const N_Integer& n)
	{
		PENSURE(positive(n));

		if (!negative(x))
		{
			// Since both numbers are non-negative,
			// the % already does what we want.
			return x % n;
		}
		
		if (negative(-x))
		{
			// x = -2^B

			// This has to be handled specially,
			// since -2^B does not have a negation.

			// The multiplication must be done using
			// negative numbers, since the result may
			// be -2^B.
			return (-divideInfinity<Integer>(n)) * n - x;
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
	Real realMod(const Real& x)
	{
		PASTEL_CONCEPT_CHECK(Real, Real_Concept);

		return x - floor(x);
	}

	template <typename Real>
	Real realMod(const Real& x, const Real& n)
	{
		PASTEL_CONCEPT_CHECK(Real, Real_Concept);

		PENSURE_OP(n, >, 0);
		return Pastel::realMod(x / n) * n;
	}

}

#endif
