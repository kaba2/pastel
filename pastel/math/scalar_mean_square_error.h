// Description: Mean square-error between scalar sequences
// Documentation: statistics.txt

#ifndef PASTELMATH_SCALAR_MEAN_SQUARE_ERROR_H
#define PASTELMATH_SCALAR_MEAN_SQUARE_ERROR_H

#include "pastel/sys/real_concept.h"
#include "pastel/sys/input_concept.h"

namespace Pastel
{

	//! Returns the mean square-error between scalar sequences.
	/*!
	Time complexity: 
	O(n)
	where n = min(size(aSet), size(bSet)).

	returns:
	[sum_{i = 1}^n (x_i - y_i)^2] / n, if n > 0,
	0, otherwise.
	*/
	template <
		typename Real, 
		typename A_Real_Input,
		typename B_Real_Input>
	Real scalarMeanSquareError(
		A_Real_Input aSet,
		B_Real_Input bSet);

}

#include "pastel/math/scalar_mean_square_error.hpp"

#endif


