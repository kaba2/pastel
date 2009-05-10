#ifndef PASTELGEOMETRY_PROJECTAXIS_HPP
#define PASTELGEOMETRY_PROJECTAXIS_HPP

#include "pastel/geometry/projectaxis.h"

#include "pastel/math/linear.h"
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
		PENSURE(box.dimension() == unitAxis.dimension());

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

}

#endif
