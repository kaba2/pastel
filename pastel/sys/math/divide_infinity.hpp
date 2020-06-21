#ifndef PASTELSYS_DIVIDE_INFINITY_HPP
#define PASTELSYS_DIVIDE_INFINITY_HPP

#include "pastel/sys/math/divide_infinity.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <
		Integer_Concept_ Integer,
		Integer_Concept_ N_Integer
	>
	Integer divideInfinity(const N_Integer& n)
	{
		PENSURE_OP(n, >=, 2);

		// Let w = SizeInBits<uinteger>::value.

		if (negative(Integer(1)))
		{
			// The number is 2-bit signed;
			// the result is zero.
			return Integer(0);
		}

		if (odd(n))
		{
			// Suppose n is odd. Then n does not divide 2^w. 
			// Therefore there exists a, b in NN 
			// such that
			//
			//     2^w = a n + b, 0 < b < n
			//     <=>
			//     2^w - 1 = a n + (b - 1),  0 <= b - 1 < n - 1 < n.
			//
			// Therefore 
			//
			//     floor(2^w / n) = a = floor((2^w - 1) / n).

			return (Integer)Infinity() / n;
		}

		// Suppose n is even. Then
		// 
		//     floor(2^w / n)
		//   = floor(2^{w - 1} / (n / 2))
		//   = floor([floor((2^w - 1) / 2) + 1] / (n / 2))

		return (((Integer)Infinity() >> 1) + 1) / (n >> 1);
	}

}

#endif
