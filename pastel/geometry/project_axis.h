// Description: Tools to orthogonally project shapes onto a line
// Documentation: pastelgeometry.txt

#ifndef PASTEL_PROJECT_AXIS_H
#define PASTEL_PROJECT_AXIS_H

#include "pastel/sys/tuple.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/sphere.h"
#include "pastel/geometry/triangle.h"

namespace Pastel
{

	//! Orthogonally projects an aligned box to an origin passing line.
	/*!
	box:
	The aligned box to project.

	unitAxis:
	The direction of the line given
	by a normalized direction vector.
	The line is then parametrized by
	L(t) = Point<N, Real>(0) + t * unitAxis

	returns:
	The interval [tMin, tMax] on line L
	that the aligned box projects to.
	*/

	template <int N, typename Real>
	AlignedBox<1, Real> projectAxis(
		const AlignedBox<N, Real>& box,
		const Vector<N, Real>& unitAxis);

	//! Orthogonally projects a box to an origin passing line.
	/*!
	box:
	The box to project.

	unitAxis:
	The direction of the line given
	by a normalized direction vector.
	The line is then parametrized by
	L(t) = Point<N, Real>(0) + t * unitAxis

	returns:
	The interval [tMin, tMax] on line L
	that the box projects to.
	*/

	template <int N, typename Real>
	AlignedBox<1, Real> projectAxis(
		const Box<N, Real>& box,
		const Vector<N, Real>& unitAxis);

	//! Orthogonally projects a sphere to an origin passing line.
	/*!
	sphere:
	The sphere to project.

	unitAxis:
	The direction of the line given
	by a normalized direction vector.
	The line is then parametrized by
	L(t) = Point<N, Real>(0) + t * unitAxis

	returns:
	The interval [tMin, tMax] on line L
	that the sphere projects to.
	*/

	template <int N, typename Real>
	AlignedBox<1, Real> projectAxis(
		const Sphere<N, Real>& sphere,
		const Vector<N, Real>& unitAxis);

	//! Orthogonally projects a line segment to an origin passing line.
	/*!
	segment:
	The line segment to project.

	unitAxis:
	The direction of the line given
	by a normalized direction vector.
	The line is then parametrized by
	L(t) = Point<N, Real>(0) + t * unitAxis

	returns:
	The interval [tMin, tMax] on line L
	that the line segment projects to.
	*/

	template <int N, typename Real>
	AlignedBox<1, Real> projectAxis(
		const Segment<N, Real>& segment,
		const Vector<N, Real>& unitAxis);

	//! Orthogonally projects a triangle to an origin passing line.
	/*!
	triangle:
	The triangle segment to project.

	unitAxis:
	The direction of the line given
	by a normalized direction vector.
	The line is then parametrized by
	L(t) = Point<N, Real>(0) + t * unitAxis

	returns:
	The interval [tMin, tMax] on line L
	that the line segment projects to.
	*/

	template <int N, typename Real>
	AlignedBox<1, Real> projectAxis(
		const Triangle<N, Real>& triangle,
		const Vector<N, Real>& unitAxis);

}

#include "pastel/geometry/project_axis.hpp"

#endif
