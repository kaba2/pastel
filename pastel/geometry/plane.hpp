#ifndef PASTEL_PLANE_HPP
#define PASTEL_PLANE_HPP

#include "pastel/geometry/plane.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <typename Real, int N>
	void swap(Plane<Real, N>& left,
		Plane<Real, N>& right)
	{
		left.swap(right);
	}

	template <typename Real, int N>
	Plane<Real, N>::Plane()
		: position_()
		, normal_(unitAxis<Real, N>(0))
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
	}

	template <typename Real, int N>
	Plane<Real, N>::Plane(integer dimension)
		: position_(ofDimension(dimension))
		, normal_(unitAxis<Real, N>(dimension, 0))
	{
	}

	template <typename Real, int N>
	Plane<Real, N>::Plane(
		const Vector<Real, N>& position,
		const Vector<Real, N>& unitNormal)
		: position_(position)
		, normal_(unitNormal)
	{
	}

	template <typename Real, int N>
	Plane<Real, N>::Plane(
		integer dimension,
		const Vector<Real, N>& position,
		const Vector<Real, N>& unitNormal)
		: position_(position)
		, normal_(unitNormal)
	{
		PENSURE_OP(dimension, ==, position.dimension());
		PENSURE_OP(dimension, ==, unitNormal.dimension());
	}

	template <typename Real, int N>
	Plane<Real, N>::~Plane()
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <typename Real, int N>
	void Plane<Real, N>::swap(
		Plane<Real, N>& that)
	{
		position_.swap(that.position_);
		normal_.swap(that.normal_);
	}

	template <typename Real, int N>
	integer Plane<Real, N>::dimension() const
	{
		return position_.dimension();
	}

	template <typename Real, int N>
	void Plane<Real, N>::set(
		const Vector<Real, N>& position,
		const Vector<Real, N>& unitNormal)
	{
		setPosition(position);
		setNormal(unitNormal);
	}

	template <typename Real, int N>
	void Plane<Real, N>::setPosition(
		const Vector<Real, N>& position)
	{
		position_ = position;
	}

	template <typename Real, int N>
	const Vector<Real, N>& Plane<Real, N>::position() const
	{
		return position_;
	}

	template <typename Real, int N>
	void Plane<Real, N>::setNormal(
		const Vector<Real, N>& unitNormal)
	{
		normal_ = unitNormal;
	}

	template <typename Real, int N>
	const Vector<Real, N>& Plane<Real, N>::normal() const
	{
		return normal_;
	}

	template <typename Real, int N>
	Plane<Real, N>& Plane<Real, N>::operator+=(
		const Vector<Real, N>& that)
	{
		position_ += that;

		return *this;
	}

	template <typename Real, int N>
	Plane<Real, N>& Plane<Real, N>::operator-=(
		const Vector<Real, N>& that)
	{
		position_ -= that;

		return *this;
	}

	template <typename Real, int N>
	Plane<Real, N>& Plane<Real, N>::operator*=(
		const Real& that)
	{
		// Do nothing.
		
		unused(that);

		return *this;
	}

	template <typename Real, int N>
	Plane<Real, N>& Plane<Real, N>::operator/=(
		const Real& that)
	{
		PENSURE_OP(that, !=, 0);

		// Do nothing.

		return *this;
	}

}

#endif
