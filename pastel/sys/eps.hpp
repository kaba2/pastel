#ifndef PASTELSYS_EPS_HPP
#define PASTELSYS_EPS_HPP

#include "pastel/sys/eps.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/log.h"
#include "pastel/sys/isnan.h"

namespace Pastel
{

	inline real64_ieee nextGreater(real64_ieee that)
	{
		// An ieee floating point number consists of the 
		// bits:
		//
		// s eee...eee fff...fff
		//
		// where
		// s is the sign bit
		// e are the exponent bits
		// f are the fraction bits

		// The main cases are:
		// exponent == 0: subnormal number
		// exponent != 0: normal number
		//
		// More specifically:
		// exponent == 0 and fraction == 0: +/- 0
		// exponent == 2^exponentBits - 1 and fraction == 0: +/- infinity
		// exponent == 2^exponentBits - 1 and fraction != 0: NaN
		//
		// Subnormal numbers are evaluated by:
		// (-1)^s 2^-(exponentBias - 1) 0.fraction
		//
		// While normal numbers are evaluated by:
		// (-1)^s 2^(exponent - exponentBias) 1.fraction
		//
		// where
		// exponentBias = 2^(exponentBits - 1) - 1

		// This is how we are going to work.
		// We will interpret and reconstruct the floating
		// point number directly using the bit representation.
		//
		// 1) If the input is a NaN, infinity or -infinity, 
		//    return it without changes
		// 2) If the input is -0, return 0. The -0 can't be handled
		//    as other subnormal numbers.
		// 3) If the number is a subnormal and the sign 
		//    bit is zero, add one to the bit representation.
		// 4) If the number is a subnormal (!= 0) and the sign 
		//    bit is one, subtract one from the bit representation.
		//
		// Convince yourself that all the following transitions
		// work correctly:
		//
		// -Infinity -> -Infinity (1)
		// Greatest negative normal -> Smallest negative subnormal (4)
		// Greatest negative subnormal (!= 0) -> -Zero (4)
		// -Zero -> +Zero
		// +Zero -> Smallest positive subnormal (2)
		// Greatest positive subnormal -> Smallest positive normal (3)
		// Greatest positive normal -> Infinity (3)
		// Infinity -> Infinity (1)
		// Nan -> Nan (1)

		if (isNan(that) ||
			that == infinity<real64_ieee>() ||
			that == -infinity<real64_ieee>())
		{
			return that;			
		}

		uint64 bits = *((uint64*)&that);
		const uint64 lastBit = (uint64)1 << 63;
		const uint64 signBit = bits & lastBit;

		if (bits == lastBit)
		{
			// -Zero -> +Zero, case 2.
			bits = 0;
		}
		else
		{		
			if (signBit != 0)
			{
				// Negative, case 4.
				--bits;
			}
			else
			{
				// Positive, case 3.
				++bits;
			}
		}

		const real64_ieee result = *((real64_ieee*)&bits);
		return result;
	}

	inline real64_ieee nextSmaller(real64_ieee that)
	{
		if (isNan(that) ||
			that == infinity<real64_ieee>() ||
			that == -infinity<real64_ieee>())
		{
			return that;			
		}

		// See the documentation for nextGreater().
		// The only difference is in the direction
		// of movement.

		uint64 bits = *((uint64*)&that);
		const uint64 lastBit = (uint64)1 << 63;
		const uint64 signBit = bits & lastBit;

		if (bits == 0)
		{
			// +Zero -> -Zero.
			bits = lastBit;
		}
		else
		{		
			if (signBit != 0)
			{
				// Negative.
				++bits;
			}
			else
			{
				// Positive.
				--bits;
			}
		}

		const real64_ieee result = *((real64_ieee*)&bits);
		return result;
	}

	inline real32_ieee nextGreater(real32_ieee that)
	{
		if (isNan(that) ||
			that == infinity<real32_ieee>() ||
			that == -infinity<real32_ieee>())
		{
			return that;			
		}

		uint32 bits = *((uint32*)&that);
		const uint32 lastBit = (uint32)1 << 31;
		const uint32 signBit = bits & lastBit;

		if (bits == lastBit)
		{
			// -Zero -> +Zero.
			bits = 0;
		}
		else
		{		
			if (signBit != 0)
			{
				// Negative.
				--bits;
			}
			else
			{
				// Positive.
				++bits;
			}
		}

		const real32_ieee result = *((real32_ieee*)&bits);
		return result;
	}

	inline real32_ieee nextSmaller(real32_ieee that)
	{
		if (isNan(that) ||
			that == infinity<real32_ieee>() ||
			that == -infinity<real32_ieee>())
		{
			return that;			
		}

		// See the documentation for nextGreater().
		// The only difference is in the direction
		// of movement.

		uint32 bits = *((uint32*)&that);
		const uint32 lastBit = (uint32)1 << 31;
		const uint32 signBit = bits & lastBit;

		if (bits == 0)
		{
			// +Zero -> -Zero.
			bits = lastBit;
		}
		else
		{		
			if (signBit != 0)
			{
				// Negative.
				++bits;
			}
			else
			{
				// Positive.
				--bits;
			}
		}

		const real32_ieee result = *((real32_ieee*)&bits);
		return result;
	}

}

#endif
