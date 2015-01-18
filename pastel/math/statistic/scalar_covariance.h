// Description: Covariance between scalar sequences
// Documentation: statistics.txt

#ifndef PASTELMATH_SCALAR_COVARIANCE_H
#define PASTELMATH_SCALAR_COVARIANCE_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/input/input_concept.h"

namespace Pastel
{

	//! Returns the covariance between scalar sequences.
	/*!
	Time complexity: 
	O(n)
	where n = min(size(aSet), size(bSet)).

	returns:
	[sum_{i = 1}^n (a_i - m_a) * (b_i - m_b)] / n, if biased and n > 0,
	[sum_{i = 1}^n (a_i - m_a) * (b_i - m_b)] / (n - 1), if !biased and n > 1,
	0, otherwise.
	*/
	template <
		typename Real,
		typename A_Real_Input,
		typename B_Real_Input>
	Real scalarCovariance(
		A_Real_Input aSet,
		B_Real_Input bSet,
		bool biased,
		const NoDeduction<Real>& aMean,
		const NoDeduction<Real>& bMean);

	//! Returns the covariance between scalar sequences.
	/*!
	This is a convenience function which returns
	scalarCovariance<Real>(
		aSet, bSet, biased, 
		scalarMean<Real>(aSet),
		scalarMean<Real>(bSet)).
	*/
	template <
		typename Real,
		typename A_Real_Input,
		typename B_Real_Input>
	Real scalarCovariance(
		A_Real_Input aSet,
		B_Real_Input bSet,
		bool biased = true);

}

#include "pastel/math/statistic/scalar_covariance.hpp"

#endif
