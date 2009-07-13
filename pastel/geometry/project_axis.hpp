#ifndef PASTEL_PROJECT_AXIS_HPP
#define PASTEL_PROJECT_AXIS_HPP

#include "pastel/geometry/project_axis.h"

#include "pastel/sys/math_functions.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	AlignedBox<1, Real> projectAxis(
		const AlignedBox<N, Real>& box,
		const Vector<N, Real>& unitAxis)
	{
		const Real t1 = dot(unitAxis, asVector(box.min()));
		const Real t2 = dot(unitAxis, asVector(box.max()));

		return AlignedBox<1, Real>(
			std::min(t1, t2),
			std::max(t1, t2));
	}

	template <int N, typename Real>
	AlignedBox<1, Real> projectAxis(
		const Box<N, Real>& box,
		const Vector<N, Real>& unitAxis)
	{
		PENSURE_OP(box.dimension(), ==, unitAxis.dimension());

		const integer dimension = box.dimension();

		const Real position = dot(unitAxis, asVector(box.position()));

		Real radius = 0;
		for (integer i = 0;i < dimension;++i)
		{
			radius += mabs(dot(box.rotation()[i] * box.width()[i], unitAxis));
		}

		return AlignedBox<1, Real>(
			position - radius,
			position + radius);
	}

	template <int N, typename Real>
	AlignedBox<1, Real> projectAxis(
		const Sphere<N, Real>& sphere,
		const Vector<N, Real>& unitAxis)
	{
		const Real position = dot(unitAxis, asVector(sphere.position()));
		const Real radius = sphere.radius();

		return AlignedBox<1, Real>(
			position - radius,
			position + radius);
	}

	template <int N, typename Real>
	AlignedBox<1, Real> projectAxis(
		const Segment<N, Real>& segment,
		const Vector<N, Real>& unitAxis)
	{
		const Vector<N, Real> delta = segment.end() - segment.start();
		const Real position = dot(unitAxis,
			linear(asVector(segment.start()), asVector(segment.end()), 0.5));
		const Real radius = mabs(dot(unitAxis, delta)) * 0.5;

		return AlignedBox<1, Real>(
			position - radius,
			position + radius);
	}

	template <int N, typename Real>
	AlignedBox<1, Real> projectAxis(
		const Triangle<N, Real>& triangle,
		const Vector<N, Real>& unitAxis)
	{
		const Real d0 = dot(asVector(triangle[0]), unitAxis);
		const Real d1 = dot(asVector(triangle[1]), unitAxis);
		const Real d2 = dot(asVector(triangle[2]), unitAxis);

		// Find out the min-max range of the
		// parameters.

		AlignedBox<1, Real> dBound;

		minMax(d0, d1, d2, dBound.min()[0], dBound.max()[0]);

		return dBound;
	}

}

#endif
