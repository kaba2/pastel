// Description: Triangle-distributed random numbers

#ifndef PASTEL_RANDOM_TRIANGLE_H
#define PASTEL_RANDOM_TRIANGLE_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{


	//! Returns a Triangle-distributed random real.
	/*!
	This function assumes that
	min = -1, mode = 0, and max = 1.
	*/
	template <typename Real>
	Real randomTriangle();

	//! Returns a Triangle-distributed random real.
	/*!
	Preconditions:
	min < mode
	mode < max
	*/
	template <typename Real>
	Real randomTriangle(
		const PASTEL_NO_DEDUCTION(Real)& min,
		const PASTEL_NO_DEDUCTION(Real)& mode,
		const PASTEL_NO_DEDUCTION(Real)& max);

	//! Probability density function of the Triangle distribution.
	template <typename Real>
	Real trianglePdf(
		const PASTEL_NO_DEDUCTION(Real)& x);

	//! Probability density function of the Triangle distribution.
	template <typename Real>
	Real trianglePdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& min,
		const PASTEL_NO_DEDUCTION(Real)& mode,
		const PASTEL_NO_DEDUCTION(Real)& max);

}

#include "pastel/sys/random_triangle.hpp"

#endif
