// Description: An aggregate file mathematical functions

#ifndef PASTELSYS_MATH_FUNCTIONS_H
#define PASTELSYS_MATH_FUNCTIONS_H

#include "pastel/sys/math/angles.h"
#include "pastel/sys/math/bernstein.h"
#include "pastel/sys/math/beta.h"
#include "pastel/sys/math/binomial.h"
#include "pastel/sys/math/clamp.h"
#include "pastel/sys/math/digamma.h"
#include "pastel/sys/math/eps.h"
#include "pastel/sys/math/error.h"
#include "pastel/sys/math/factorial.h"
#include "pastel/sys/math/gamma.h"
#include "pastel/sys/math/harmonic.h"
#include "pastel/sys/math/linear.h"
#include "pastel/sys/math/logarithm.h"
#include "pastel/sys/math/minmax.h"
#include "pastel/sys/math/mod.h"
#include "pastel/sys/math/number_tests.h"
#include "pastel/sys/math/powers.h"
#include "pastel/sys/math/quadratic.h"
#include "pastel/sys/math/quantization.h"
#include "pastel/sys/math/rounding.h"
#include "pastel/sys/integer/scale_integer.h"
#include "pastel/sys/math/sign.h"
#include "pastel/sys/math/sinc.h"

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
