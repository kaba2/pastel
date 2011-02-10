// Description: Triangle-distributed random numbers
// Documentation: random_continuous.txt

#ifndef PASTEL_RANDOM_TRIANGLE_H
#define PASTEL_RANDOM_TRIANGLE_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{


	//! Returns a triangle-distributed random real.
	/*!
	This function assumes that
	min = -1, mode = 0, and max = 1.
	*/
	template <typename Real>
	Real randomTriangle();

	//! Returns a triangle-distributed random real.
	/*!
	Preconditions:
	leftWidth > 0
	rightWidth > 0
	*/
	template <typename Real>
	Real randomTriangle(
		const PASTEL_NO_DEDUCTION(Real)& leftWidth,
		const PASTEL_NO_DEDUCTION(Real)& rightWidth);

	//! Probability density function of the Triangle distribution.
	template <typename Real>
	Real trianglePdf(
		const PASTEL_NO_DEDUCTION(Real)& x);

	//! Probability density function of the Triangle distribution.
	/*!
	Preconditions:
	leftWidth > 0
	rightWidth > 0
	*/
	template <typename Real>
	Real trianglePdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& leftWidth,
		const PASTEL_NO_DEDUCTION(Real)& rightWidth);

}

#include "pastel/sys/random_triangle.hpp"

#endif
