#ifndef PASTEL_PLANE_HPP
#define PASTEL_PLANE_HPP

#include "pastel/geometry/plane.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <int N, typename Real>
	void swap(Plane<N, Real>& left,
		Plane<N, Real>& right)
	{
		left.swap(right);
	}

	template <int N, typename Real>
	Plane<N, Real>::Plane()
		: position_()
		, normal_(unitAxis<N, Real>(0))
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
	}

	template <int N, typename Real>
	Plane<N, Real>::Plane(integer dimension)
		: position_(ofDimension(dimension))
		, normal_(unitAxis<N, Real>(dimension, 0))
	{
	}

	template <int N, typename Real>
	Plane<N, Real>::Plane(
		const Point<N, Real>& position,
		const Vector<N, Real>& unitNormal)
		: position_(position)
		, normal_(unitNormal)
	{
	}

	template <int N, typename Real>
	Plane<N, Real>::Plane(
		integer dimension,
		const Point<N, Real>& position,
		const Vector<N, Real>& unitNormal)
		: position_(position)
		, normal_(unitNormal)
	{
		PENSURE_OP(dimension, ==, position.dimension());
		PENSURE_OP(dimension, ==, unitNormal.dimension());
	}

	template <int N, typename Real>
	Plane<N, Real>::~Plane()
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <int N, typename Real>
	void Plane<N, Real>::swap(
		Plane<N, Real>& that)
	{
		position_.swap(that.position_);
		normal_.swap(that.normal_);
	}

	template <int N, typename Real>
	integer Plane<N, Real>::dimension() const
	{
		return position_.dimension();
	}

	template <int N, typename Real>
	void Plane<N, Real>::set(
		const Point<N, Real>& position,
		const Vector<N, Real>& unitNormal)
	{
		setPosition(position);
		setNormal(unitNormal);
	}

	template <int N, typename Real>
	void Plane<N, Real>::setPosition(
		const Point<N, Real>& position)
	{
		position_ = position;
	}

	template <int N, typename Real>
	const Point<N, Real>& Plane<N, Real>::position() const
	{
		return position_;
	}

	template <int N, typename Real>
	void Plane<N, Real>::setNormal(
		const Vector<N, Real>& unitNormal)
	{
		normal_ = unitNormal;
	}

	template <int N, typename Real>
	const Vector<N, Real>& Plane<N, Real>::normal() const
	{
		return normal_;
	}

}

#endif
