#ifndef PASTELMATH_SPHERICALLINEAR_HPP
#define PASTELMATH_SPHERICALLINEAR_HPP

#include "pastel/math/sphericallinear.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Vector<N, Real> sphericalLinear(
		const Vector<N, Real>& unitFrom,
		const Vector<N, Real>& unitTo,
		const Real& time)
	{
		const Real alpha(acos(dot(unitFrom, unitTo)));
		const Real invSinAlpha(inverse(sin(alpha)));
		const Real fromFactor(sin((1 - time) * alpha) *
			invSinAlpha);
		const Real toFactor(sin(time * alpha) *
			invSinAlpha);

		return fromFactor * unitFrom + toFactor * unitTo;
	}

}

#endif
