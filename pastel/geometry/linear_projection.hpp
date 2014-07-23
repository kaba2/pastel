#ifndef PASTELGEOMETRY_LINEAR_PROJECTION_HPP
#define PASTELGEOMETRY_LINEAR_PROJECTION_HPP

#include "pastel/geometry/linear_projection.h"

#include "pastel/sys/math_functions.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	AlignedBox<Real, 1> projectAxis(
		const AlignedBox<Real, N>& box,
		const Vector<Real, N>& unitAxis)
	{
		Real t1 = dot(unitAxis, box.min());
		const Real t2 = dot(unitAxis, box.max());

		return AlignedBox<Real, 1>(
			std::min(t1, t2),
			std::max(t1, t2));
	}

	template <typename Real, int N>
	AlignedBox<Real, 1> projectAxis(

		const Box<Real, N>& box,
		const Vector<Real, N>& unitAxis)
	{
		PENSURE_OP(box.n(), ==, unitAxis.n());

		integer dimension = box.n();

		const Real position = dot(unitAxis, box.position());

		Real radius = 0;
		for (integer i = 0;i < dimension;++i)
		{

			radius += mabs(dot(box.rotation()[i] * box.width()[i], unitAxis));
		}

		return AlignedBox<Real, 1>(
			position - radius,
			position + radius);
	}

	template <typename Real, int N>
	AlignedBox<Real, 1> projectAxis(
		const Sphere<Real, N>& sphere,
		const Vector<Real, N>& unitAxis)
	{
		Real position = dot(unitAxis, sphere.position());
		const Real radius = sphere.radius();

		return AlignedBox<Real, 1>(
			position - radius,
			position + radius);
	}

	template <typename Real, int N>
	AlignedBox<Real, 1> projectAxis(

		const Segment<Real, N>& segment,
		const Vector<Real, N>& unitAxis)
	{
		Vector<Real, N> delta = segment.end() - segment.start();
		const Real position = dot(unitAxis,
			linear(segment.start(), segment.end(), 0.5));

		const Real radius = mabs(dot(unitAxis, delta)) * 0.5;

		return AlignedBox<Real, 1>(
			position - radius,
			position + radius);
	}

	template <typename Real, int N>
	AlignedBox<Real, 1> projectAxis(
		const PASTEL_TRIANGLE(Real, N)& triangle,
		const Vector<Real, N>& unitAxis)
	{
		Real d0 = dot(triangle[0], unitAxis);
		const Real d1 = dot(triangle[1], unitAxis);
		const Real d2 = dot(triangle[2], unitAxis);

		// Find out the min-max range of the
		// parameters.

		AlignedBox<Real, 1> dBound;

		minMax(d0, d1, d2, dBound.min()[0], dBound.max()[0]);

		return dBound;
	}

}

#endif
