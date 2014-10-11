// Description: An aggregate file mathematical functions

#ifndef PASTELSYS_MATH_FUNCTIONS_H
#define PASTELSYS_MATH_FUNCTIONS_H

#include "pastel/sys/angles.h"
#include "pastel/sys/bernstein.h"
#include "pastel/sys/beta.h"
#include "pastel/sys/binomial.h"
#include "pastel/sys/clamp.h"
#include "pastel/sys/digamma.h"
#include "pastel/sys/eps.h"
#include "pastel/sys/error.h"
#include "pastel/sys/factorial.h"
#include "pastel/sys/gamma.h"
#include "pastel/sys/harmonic.h"
#include "pastel/sys/lexicographic.h"
#include "pastel/sys/linear.h"
#include "pastel/sys/logarithm.h"
#include "pastel/sys/minmax.h"
#include "pastel/sys/mod.h"
#include "pastel/sys/number_tests.h"
#include "pastel/sys/powers.h"
#include "pastel/sys/quadratic.h"
#include "pastel/sys/quantization.h"
#include "pastel/sys/rounding.h"
#include "pastel/sys/scale_integer.h"
#include "pastel/sys/sign.h"
#include "pastel/sys/sinc.h"
#include "pastel/sys/tristate.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Returns the [signed area of a triangle].
	/*!
	If the three points are collinear,
	the returned value is zero.
	If the three points form a counter-clockwise
	turn, then the returned value is positive.
	Otherwise the points form a clockwise turn
	and the returned value is negative.
	The absolute value of the returned value
	is the area of the triangle.
	*/
	template <typename Real>
	Real signedArea(
		const Vector<Real, 2>& a,
		const Vector<Real, 2>& b,
		const Vector<Real, 2>& c);

	template <typename Real>
	void realToReal(
		const Real& x,
		const NoDeduction<Real>& fromMin, 
		const NoDeduction<Real>& fromMax,
		const NoDeduction<Real>& toMin, 
		const NoDeduction<Real>& toMax);

}

#include "pastel/sys/math_functions.hpp"

#endif
