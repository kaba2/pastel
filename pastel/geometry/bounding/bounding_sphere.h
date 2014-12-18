// Description: An aggregate file for bounding spheres

#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_H
#define PASTELGEOMETRY_BOUNDING_SPHERE_H

#include "pastel/geometry/bounding/bounding_sphere_alignedbox.h"
#include "pastel/geometry/bounding/bounding_sphere_box.h"
#include "pastel/geometry/bounding/bounding_sphere_segment.h"
#include "pastel/geometry/bounding/bounding_sphere_pointset.h"

#include "pastel/geometry/sphere.h"
#include "pastel/geometry/simplex.h"

namespace Pastel
{

	//! Finds the minimum volume bounding sphere of a 0-simplex.
	template <typename Real, int N>
	Sphere<Real, N> boundingSphere(
		const PASTEL_SIMPLEX(Real, N, 0)& simplex);

	//! Finds the minimum volume bounding sphere of a 1-simplex.

	template <typename Real, int N>
	Sphere<Real, N> boundingSphere(
		const PASTEL_SIMPLEX(Real, N, 1)& simplex);

	//! Finds the minimum volume bounding sphere of an M-simplex.

	template <typename Real, int N, int M>
	Sphere<Real, N> boundingSphere(
		const PASTEL_SIMPLEX(Real, N, M)& simplex);

	//! Finds the circumscribed sphere of a point.

	template <typename Real, int N>
	Sphere<Real, N> circumscribedSphere(
		const Vector<Real, N>& aPoint);

	//! Finds the circumscribed sphere of two points.

	template <typename Real, int N>
	Sphere<Real, N> circumscribedSphere(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint);

	//! Finds the circumscribed sphere of a 0-simplex.

	template <typename Real, int N>
	Sphere<Real, N> circumscribedSphere(
		const PASTEL_SIMPLEX(Real, N, 0)& simplex);

	//! Finds the circumscribed sphere of a 1-simplex.

	template <typename Real, int N>
	Sphere<Real, N> circumscribedSphere(
		const PASTEL_SIMPLEX(Real, N, 1)& simplex);

	//! Finds the circumscribed sphere of an M-simplex.

	template <typename Real, int N, int M>
	Sphere<Real, N> circumscribedSphere(
		const PASTEL_SIMPLEX(Real, N, M)& simplex);

}

#include "pastel/geometry/bounding/bounding_sphere.hpp"

#endif
