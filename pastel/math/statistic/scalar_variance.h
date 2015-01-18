// Description: Variance of a scalar set
// Documentation: statistics.txt

#ifndef PASTELMATH_SCALAR_VARIANCE_H
#define PASTELMATH_SCALAR_VARIANCE_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/input/input_concept.h"

#include "pastel/math/statistic/scalar_mean.h"

namespace Pastel
{

	//! Returns the variance of a scalar set.
	/*!
	Time complexity: 
	O(n)
	where n = size(inputSet).

	returns:
	[sum_{i = 1}^n (x_i - mean)^2] / n, if biased and n > 0,
	[sum_{i = 1}^n (x_i - mean)^2] / (n - 1), if !biased and n > 1,
	0, otherwise.
	*/
	template <
		typename Real,
		typename Real_Input>
	Real scalarVariance(
		Real_Input inputSet,
		bool biased,
		const NoDeduction<Real>& mean);

	//! Returns the variance of a scalar set.
	/*!
	This is a convenience function which returns
	scalarVariance(inputSet, biased, scalarMean<Real>(inputSet)).
	*/
	template <
		typename Real,
		typename Real_Input>
	Real scalarVariance(
		Real_Input inputSet,
		bool biased = true);

}

#include "pastel/math/statistic/scalar_variance.hpp"

#endif
