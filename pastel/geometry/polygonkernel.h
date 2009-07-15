// Description: Tools to compute the kernel of a polygon
// Documentation: pastelgeometry.txt

#ifndef PASTEL_POLYGONKERNEL_H
#define PASTEL_POLYGONKERNEL_H

#include "pastel/geometry/polygon.h"
#include "pastel/geometry/plane.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Clips a polygon with a half-space.
	/*!
	That half-space to which the plane normal does not
	point to is cut away arom the polygon.
	*/
	template <integer N, typename Real>
	Polygon<N, Real> clip(
		const Polygon<N, Real>& input,
		const Plane<N, Real>& plane);

	//! Computes the kernel of a polygon.
	/*!
	The kernel of a polygon is the set of those points
	relative to which the polygon is star-convex.
	*/
	template <typename Real>
	Polygon<2, Real> polygonKernel(const Polygon<2, Real>& input);

}

#include "pastel/geometry/polygonkernel.hpp"

#endif
