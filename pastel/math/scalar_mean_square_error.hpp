#ifndef PASTELMATH_SCALAR_MEAN_SQUARE_ERROR_HPP
#define PASTELMATH_SCALAR_MEAN_SQUARE_ERROR_HPP

#include "pastel/math/scalar_mean_square_error.h"
#include "pastel/sys/powers.h"

namespace Pastel
{

	template <
		typename Real, 
		typename A_Real_Input, 
		typename B_Real_Input>
	Real scalarMeanSquareError(
		A_Real_Input aSet,
		B_Real_Input bSet)
	{
		integer n = 0;
		Real result = 0;

		while (!aSet.empty() && !bSet.empty())
		{
			result += square(aSet.get() - bSet.get());
			++n;

			aSet.pop();
			bSet.pop();
		}

		if (n == 0)
		{
			return 0;
		}

		return result / n;
	}
	

}

#endif
