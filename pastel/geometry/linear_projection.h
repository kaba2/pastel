// Description: Projections onto a line
// Documentation: projection.txt

#ifndef PASTEL_LINEAR_PROJECTION_H
#define PASTEL_LINEAR_PROJECTION_H

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
	L(t) = Vector<Real, N>(0) + t * unitAxis

	returns:
	The interval [tMin, tMax] on line L
	that the aligned box projects to.
	*/

	template <typename Real, int N>
	AlignedBox<Real, 1> projectAxis(
		const AlignedBox<Real, N>& box,
		const Vector<Real, N>& unitAxis);

	//! Orthogonally projects a box to an origin passing line.
	/*!
	box:
	The box to project.

	unitAxis:
	The direction of the line given
	by a normalized direction vector.
	The line is then parametrized by
	L(t) = Vector<Real, N>(0) + t * unitAxis

	returns:
	The interval [tMin, tMax] on line L
	that the box projects to.
	*/

	template <typename Real, int N>
	AlignedBox<Real, 1> projectAxis(
		const Box<Real, N>& box,
		const Vector<Real, N>& unitAxis);

	//! Orthogonally projects a sphere to an origin passing line.
	/*!
	sphere:
	The sphere to project.

	unitAxis:
	The direction of the line given
	by a normalized direction vector.
	The line is then parametrized by
	L(t) = Vector<Real, N>(0) + t * unitAxis

	returns:
	The interval [tMin, tMax] on line L
	that the sphere projects to.
	*/

	template <typename Real, int N>
	AlignedBox<Real, 1> projectAxis(
		const Sphere<Real, N>& sphere,
		const Vector<Real, N>& unitAxis);

	//! Orthogonally projects a line segment to an origin passing line.
	/*!
	segment:
	The line segment to project.

	unitAxis:
	The direction of the line given
	by a normalized direction vector.
	The line is then parametrized by
	L(t) = Vector<Real, N>(0) + t * unitAxis

	returns:
	The interval [tMin, tMax] on line L
	that the line segment projects to.
	*/

	template <typename Real, int N>
	AlignedBox<Real, 1> projectAxis(
		const Segment<Real, N>& segment,
		const Vector<Real, N>& unitAxis);

	//! Orthogonally projects a triangle to an origin passing line.
	/*!
	triangle:
	The triangle segment to project.

	unitAxis:
	The direction of the line given
	by a normalized direction vector.
	The line is then parametrized by
	L(t) = Vector<Real, N>(0) + t * unitAxis

	returns:
	The interval [tMin, tMax] on line L
	that the line segment projects to.
	*/

	template <typename Real, int N>
	AlignedBox<Real, 1> projectAxis(
		const PASTEL_TRIANGLE(Real, N)& triangle,
		const Vector<Real, N>& unitAxis);

}

#include "pastel/geometry/linear_projection.hpp"

#endif
