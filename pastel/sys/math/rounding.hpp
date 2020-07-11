#ifndef PASTELSYS_ROUNDING_HPP
#define PASTELSYS_ROUNDING_HPP

#include "pastel/sys/math/rounding.h"
#include "pastel/sys/math/mod.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/math/number_tests.h"
#include "pastel/sys/math/powers.h"

namespace Pastel
{

	template <Integer_Concept_ Integer>
	Integer roundUpTo(
		const Integer& that, const Integer& to)
	{
		PENSURE(!negative(to));

		Integer remainder = mod(that, to);
		if (positive(remainder))
		{
			return (that - remainder) + to;
		}

		return that;
	}

	template <Integer_Concept_ Integer>
	Integer roundUpToOdd(const Integer& that)
	{
		return odd(that) ? that : (that + 1);
	}

	template <std::floating_point Real>
	integer roundUpToOdd(const Real& that)
	{
		return Pastel::roundUpToOdd((integer)ceil(that));
	}

	template <Integer_Concept_ Integer>
	Integer roundUpToEven(const Integer& that)
	{
		return even(that) ? that : (that + 1);
	}

	template <std::floating_point Real>
	integer roundUpToEven(const Real& that)
	{
		return Pastel::roundUpToEven((integer)ceil(that));
	}

	template <std::unsigned_integral Integer>
	Integer roundUpToPowerOfTwo(const Integer& that)
	{
		if (zero(that))
		{
			return 1;
		}

		Integer X = that - 1;
		for (integer i = 1; i < SizeInBits<Integer>::value;i <<= 1)
		{
			X |= X >> i;
		}

		return X + 1;
	}

	template <std::signed_integral Integer>
	Integer roundUpToPowerOfTwo(const Integer& that)
	{
		return twosComplementToSigned(
			roundUpToPowerOfTwo(signedToTwosComplement(that)));
	}

	template <Integer_Concept_ Integer>
	Integer roundUpToPowerOfTwo(
		const Integer& that, 
		integer power)
	{
		PENSURE(!negative(power));

		Integer remainder = modPowerOfTwo(that, power);
		if (positive(remainder))
		{
			return (that - remainder) + powerOfTwo<Integer>(power);
		}

		return that;
	}

}

#endif
