// Description: Bounding sphere of a line segment

#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_SEGMENT_H
#define PASTELGEOMETRY_BOUNDING_SPHERE_SEGMENT_H

#include "pastel/geometry/shape/sphere.h"
#include "pastel/geometry/shape/segment.h"
#include "pastel/geometry/bounding/bounding_sphere_pointset.h"

namespace Pastel
{

	//! Finds the minimum volume bounding sphere of a line segment.
	template <typename Real, int N>
	Sphere<Real, N> boundingSphere(
		const Segment<Real, N>& segment)
	{
		return Pastel::boundingSphere(segment.start(), segment.end());
	}

}

#endif
