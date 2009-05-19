/*!
\file
\brief A function for performing spherical linear interpolation.
*/

#ifndef PASTEL_SPHERICALLINEAR_H
#define PASTEL_SPHERICALLINEAR_H

namespace Pastel
{

	//! Interpolates unit vectors on the surface of the unit sphere.

	/*!
	The input vectors are required to be of unit length.
	Non-unit length vectors produce wrong results.
	*/

	template <int N, typename Real>
	Vector<N, Real> sphericalLinear(
		const Vector<N, Real>& unitFrom,
		const Vector<N, Real>& unitTo,
		const Real& time);

}

#include "pastel/math/slerp.hpp"

#endif
