#ifndef PASTELSYS_RANDOM_INTEGER_HPP
#define PASTELSYS_RANDOM_INTEGER_HPP

#include "pastel/sys/random_integer.h"
#include "pastel/sys/mt19937.h"
#include "pastel/sys/bit/bitmask.h"
#include "pastel/sys/divide_infinity.h"
#include "pastel/sys/math/number_tests.h"

namespace Pastel
{

	namespace Random_
	{

		template <integer Bits>
		class RandomInteger
		{
		};

		template <>
		class RandomInteger<32>
		{
		public:
			uinteger operator()() const
			{
				return Pastel::randomUint32();
			}
		};

		template <>
		class RandomInteger<64>
		{
		public:
			uinteger operator()() const
			{
				return Pastel::randomUint64();
			}
		};
	}

	inline uinteger randomUinteger()
	{
		// FIX: Replace with sizeInBits() when constexpr
		// becomes supported in Visual Studio.
		return Random_::RandomInteger<sizeof(uinteger) * CHAR_BIT>()();
	}

	inline uinteger randomUintegerBits(uinteger bits)
	{
		PENSURE_OP(bits, >, 0);
		PENSURE_OP(bits, <=, sizeInBits<uinteger>());

		uinteger result = randomUinteger();
		if (bits < sizeInBits<uinteger>())
		{
			result &= bitMask<uinteger>(bits);
		}

		return result;
	}

	inline uinteger randomUinteger(uinteger n)
	{
		// Let w = sizeInBits<uinteger>(), and m = 2^w.

		// Let X ~ Uniform([0, m)).
		// We wish to generate Y ~ Uniform([0, n)).
		// To do this, we use rejection sampling;
		// Y = (X|[0, n)).

		if (n == 0)
		{
			// The range is the whole uinteger range.
			return randomUinteger();
		}

		if (isPowerOfTwo(n))
		{
			// The n divides m. We can aliase [0, m)
			// into [0, n).
			return randomUinteger() % n;
		}

		// From now on n does not divide m.

		// Let n' = floor(m / n) n.
		// This is well-defined, since n > 2 and
		// n does not divide m.
		uinteger rejectionCutoff = divideInfinity(n) * n;
	
		// Rejection sampling.
		uinteger result = 0;
		do
		{
			// Let i be a sample from X.
			result = randomUinteger();

			// If i < n', accept the sample.
			// The probability of this happening is
			//
			//     p = n' / m.
		}
		while (result >= rejectionCutoff);

		// The number of times needed to sample X
		// is given by Y + 1, where Y ~ NB(1, 1 - p).
		// Then E(Y + 1) = 1 / p.
		//
		// The smallest p is given by n = 2^{w - 1} + 1;
		// then p = 1/2 + 2^{-w}. Therefore it always
		// holds that E(Y + 1) < 2. That is, the rejection 
		// sampling algorithm has O(1) expected time-complexity.

		// Since n divides n', and 'result'
		// is uniformly distributed on [0, n'),
		// we can aliase the numbers to [0, n).
		return result % n;
	}

	inline integer randomInteger(integer n)
	{
		PENSURE_OP(n, >=, 0);
		if (n == 0)
		{
			return (integer)(randomUinteger() >> 1);
		}

		return (integer)randomUinteger((uinteger)n);
	}

}

#endif
