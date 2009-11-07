#ifndef PASTEL_DIFFERENTIAL_GEOMETRY_H
#define PASTEL_DIFFERENTIAL_GEOMETRY_H

#include "pastel/ray/raylibrary.h"
#include "pastel/ray/localgeometry.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Computes the derivatives of a normal vector.
	/*!
	Assume a surface is parametrized by:
	p : R^2 -> R^3 : p(u, v) = (x, y, z)
	
	Its tangent vectors are given by
	dp/du and dp/dv. A normal vector
	to the surface is given by:

	N(u, v) = cross((dp/du)(u, v), (dp/dv)(u, v))

	This function computes dN/du and dN/dv.
	*/
	PASTELRAY void computeNormalDerivatives(
		const LocalGeometry& surface,
		Vector3& dnDu,
		Vector3& dnDv);

}

#endif
