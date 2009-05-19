#ifndef PASTEL_POLYGONKERNEL_H
#define PASTEL_POLYGONKERNEL_H

#include "pastel/geometry/polygon.h"
#include "pastel/geometry/plane.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <integer N, typename Real>
	Polygon<N, Real> clip(
		const Polygon<N, Real>& input,
		const Plane<N, Real>& plane);

	template <typename Real>
	Polygon<2, Real> polygonKernel(const Polygon<2, Real>& input);

}

#include "pastel/geometry/polygonkernel.hpp"

#endif
