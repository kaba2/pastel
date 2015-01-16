// Description: Triangle-distributed random numbers

#ifndef PASTELSYS_RANDOM_TRIANGLE_H
#define PASTELSYS_RANDOM_TRIANGLE_H

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
		const NoDeduction<Real>& leftWidth,
		const NoDeduction<Real>& rightWidth);

	//! Probability density function of the Triangle distribution.
	template <typename Real>
	Real trianglePdf(
		const NoDeduction<Real>& x);

	//! Probability density function of the Triangle distribution.
	/*!
	Preconditions:
	leftWidth > 0
	rightWidth > 0
	*/
	template <typename Real>
	Real trianglePdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& leftWidth,
		const NoDeduction<Real>& rightWidth);

}

#include "pastel/sys/random_triangle.hpp"

#endif
