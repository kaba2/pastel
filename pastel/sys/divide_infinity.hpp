#ifndef PASTELSYS_DIVIDE_INFINITY_HPP
#define PASTELSYS_DIVIDE_INFINITY_HPP

#include "pastel/sys/divide_infinity.h"

namespace Pastel
{

	//! Returns floor((infinity<Type>() + 1) / n).
	/*!
	Preconditions:
	n >= 2

	This computation is tricky because inf + 1 = 0
	due to two's complement arithmetic when using
	unsigned integers.
	*/
	template <typename Type>
	PASTEL_ENABLE_IF(std::is_unsigned<Type>, Type)
		divideInfinity(Type n)
	{
		PENSURE_OP(n, >=, 2);

		// Let w = sizeInBits<uinteger>().

		if (odd(n))
		{
			// Suppose n is odd. Then n does not divide 2^w. 
			// Therefore there exists a, b in NN 
			// such that
			//
			//     2^w = a n + b, 0 < b < n
			//     <=>
			//     2^w - 1 = a n + (b - 1),  0 <= b < n - 1 < n.
			//
			// Therefore 
			//
			//     floor(2^w / n) = floor((2^w - 1) / n).

			return infinity<Type>() / n;
		}

		// Suppose n is even. Then
		// 
		//     floor(2^w / n)
		//   = floor(2^{w - 1} / (n / 2))
		//   = floor([floor((2^w - 1) / 2) + 1] / (n / 2))

		return ((infinity<Type>() >> 1) + 1) / (n >> 1);
	}

}

#endif
