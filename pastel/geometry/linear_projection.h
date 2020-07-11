// Description: Projections onto a line
// Documentation: projection.txt

#ifndef PASTELGEOMETRY_LINEAR_PROJECTION_H
#define PASTELGEOMETRY_LINEAR_PROJECTION_H

#include "pastel/sys/tuple.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/math_functions.h"

#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/sphere.h"
#include "pastel/geometry/shape/triangle.h"

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
		const Vector<Real, N>& unitAxis)
	{
		Real t1 = dot(unitAxis, box.min());
		Real t2 = dot(unitAxis, box.max());

		return AlignedBox<Real, 1>(
			std::min(t1, t2),
			std::max(t1, t2));
	}

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
		const Vector<Real, N>& unitAxis)
	{
		PENSURE_OP(box.n(), ==, unitAxis.n());

		integer dimension = box.n();

		Real position = dot(unitAxis, box.position());

		Real radius = 0;
		for (integer i = 0;i < dimension;++i)
		{

			radius += abs(dot(box.rotation()[i] * box.width()[i], unitAxis));
		}

		return AlignedBox<Real, 1>(
			position - radius,
			position + radius);
	}

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
		const Vector<Real, N>& unitAxis)
	{
		Real position = dot(unitAxis, sphere.position());
		Real radius = sphere.radius();

		return AlignedBox<Real, 1>(
			position - radius,
			position + radius);
	}

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
		const Vector<Real, N>& unitAxis)
	{
		Vector<Real, N> delta = segment.end() - segment.start();
		Real position = dot(unitAxis,
			linear(segment.start(), segment.end(), 0.5));

		const Real radius = abs(dot(unitAxis, delta)) * 0.5;

		return AlignedBox<Real, 1>(
			position - radius,
			position + radius);
	}

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
		const Vector<Real, N>& unitAxis)
	{
		Real d0 = dot(triangle[0], unitAxis);
		Real d1 = dot(triangle[1], unitAxis);
		Real d2 = dot(triangle[2], unitAxis);

		// Find out the min-max range of the
		// parameters.

		AlignedBox<Real, 1> dBound;

		minMax(d0, d1, d2, dBound.min()[0], dBound.max()[0]);

		return dBound;
	}

}

#endif
