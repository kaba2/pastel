// Description: Quantization
// Documentation: math_functions.txt

#ifndef PASTELSYS_QUANTIZATION_H
#define PASTELSYS_QUANTIZATION_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Converts dreal [0, 1] to integer [0, numbers - 1].
	/*!
	[Quantization]

	x < 0 will be converted to 0.
	x > 1 will be converted to numbers - 1.

	Random uniform distributions on [0, 1] map
	to random uniform distributions on [0, numbers - 1].
	*/
	integer quantizeUnsigned(real64 x, integer numbers);

	//! Converts integer [0, numbers - 1] to dreal [0, 1].
	/*!
	x < 0 will be converted to 0.
	x > numbers - 1 will be converted to 1.

	0 maps to 0.
	numbers - 1 maps to 1.
	*/
	real64 dequantizeUnsignedMatchEnds(integer x, integer numbers);

	//! Converts integer [0, numbers - 1] to dreal [0, 1].
	/*!
	x < 0 will be converted to 0.
	x > numbers - 1 will be converted to 1.

	Error is minimized.
	*/
	real64 dequantizeUnsigned(integer x, integer numbers);

	//! Converts an integer [-2^(N - 1) , 2^(N - 1) - 1] to a dreal [-1, 1].
	/*!
	Integer values below the range will be converted to -1.
	Integer values above the range will be converted to 1.
	*/
	template <integer N>
	real64 dequantizeSigned(integer i);

	//! Converts an integer [-2^(N - 1) , 2^(N - 1) - 1] to a dreal [-1, 1].
	/*!
	Integer values below the range will be converted to -1.
	Integer values above the range will be converted to 1.
	Random dithering is applied to the result.
	*/

	/*
	template <integer N>
	real64 ditheredDequantizeSigned(integer i);
	*/

	//! Converts a dreal [-1, 1] to integer [-(2^(N - 1) - 1) , 2^(N - 1) - 1].
	/*!
	Real values below the range will be converted to -(2^(N - 1) - 1).
	Real values above the range will be converted to 2^(N - 1) - 1.
	*/
	template <integer N>
	integer quantizeSigned(real64 r);

}

#include "pastel/sys/math/quantization.hpp"

#endif
