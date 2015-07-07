#ifndef PASTELSYS_ROUNDING_HPP
#define PASTELSYS_ROUNDING_HPP

#include "pastel/sys/math/rounding.h"
#include "pastel/sys/math/mod.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/math/number_tests.h"
#include "pastel/sys/math/powers.h"

namespace Pastel
{

	template <
		typename Integer,
		Requires<Models<Integer, Integer_Concept>>>
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

	template <
		typename Integer,
		Requires<Models<Integer, Integer_Concept>>>
	Integer roundUpToOdd(const Integer& that)
	{
		PASTEL_CONCEPT_CHECK(Integer, Integer_Concept);

		return odd(that) ? that : (that + 1);
	}

	template <
		typename Real, 
		Requires<std::is_floating_point<Real>>>
	integer roundUpToOdd(const Real& that)
	{
		return Pastel::roundUpToOdd((integer)ceil(that));
	}

	template <
		typename Integer,
		Requires<Models<Integer, Integer_Concept>>>
	Integer roundUpToEven(const Integer& that)
	{
		PASTEL_CONCEPT_CHECK(Integer, Integer_Concept);

		return even(that) ? that : (that + 1);
	}

	template <
		typename Real, 
		Requires<std::is_floating_point<Real>>>
	integer roundUpToEven(const Real& that)
	{
		return Pastel::roundUpToEven((integer)ceil(that));
	}

	template <
		typename Integer, 
		Requires<std::is_unsigned<Integer>>>
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

	template <
		typename Integer, 
		Requires<std::is_signed<Integer>>>
	Integer roundUpToPowerOfTwo(const Integer& that)
	{
		return twosComplementToSigned(
			roundUpToPowerOfTwo(signedToTwosComplement(that)));
	}

	template <
		typename Integer,
		Requires<Models<Integer, Integer_Concept>>>
	Integer roundUpToPowerOfTwo(
		const Integer& that, 
		integer power)
	{
		PASTEL_CONCEPT_CHECK(Integer, Integer_Concept);

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
