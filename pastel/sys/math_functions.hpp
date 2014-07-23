#ifndef PASTELSYS_MATH_FUNCTIONS_HPP
#define PASTELSYS_MATH_FUNCTIONS_HPP

#include "pastel/sys/math_functions.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real>
	Real signedArea(
		const Vector<Real, 2>& a,
		const Vector<Real, 2>& b,
		const Vector<Real, 2>& c)
	{
		return 0.5 * dot(cross(b - a), c - a);
	}

	template <typename Real>
	void realToReal(
		const Real& x,
		const PASTEL_NO_DEDUCTION(Real)& fromMin, 
		const PASTEL_NO_DEDUCTION(Real)& fromMax,
		const PASTEL_NO_DEDUCTION(Real)& toMin, 
		const PASTEL_NO_DEDUCTION(Real)& toMax)
	{
		Real fromDelta = fromMax - fromMin;
		const Real toDelta = toMax - toMin;

		return toMin + ((x - fromMin) / fromDelta) * toDelta;
	}

}

#endif
