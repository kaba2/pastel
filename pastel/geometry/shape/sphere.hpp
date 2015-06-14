#ifndef PASTELGEOMETRY_SPHERE_HPP
#define PASTELGEOMETRY_SPHERE_HPP

#include "pastel/geometry/shape/sphere.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/math/constants.h"

namespace Pastel
{

	template <typename Real, integer N>
	Sphere<Real, N>::Sphere()
		: position_(0)
		, radius_(1)
		, inverseRadius_(1)
		, topology_(Topology::Closed)
	{
		PASTEL_STATIC_ASSERT(N != Dynamic);
	}

	template <typename Real, integer N>
	Sphere<Real, N>::Sphere(integer dimension)
		: position_(ofDimension(dimension), 0)
		, radius_(1)
		, inverseRadius_(1)
		, topology_(Topology::Closed)
	{
	}

	template <typename Real, integer N>
	Sphere<Real, N>::Sphere(
		const Vector<Real, N>& position,
		const Real& radius,
		Topology topology)
		: position_(position)
		, radius_(radius)
		, inverseRadius_(inverse(radius))
		, topology_(topology)
	{
		PASTEL_STATIC_ASSERT(N != Dynamic);
	}

	template <typename Real, integer N>
	Sphere<Real, N>::Sphere(
		integer dimension,
		const Vector<Real, N>& position,
		const Real& radius,
		Topology topology)
		: position_(position)
		, radius_(radius)
		, inverseRadius_(inverse(radius))
		, topology_(topology)
	{
		PENSURE_OP(dimension, ==, position.n());
	}

	template <typename Real, integer N>
	Sphere<Real, N>::~Sphere()
	{
		PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <typename Real, integer N>
	void Sphere<Real, N>::swap(Sphere& that)
	{
		using std::swap;
		using std::swap;

		position_.swap(that.position_);
		swap(radius_, that.radius_);
		swap(inverseRadius_, that.inverseRadius_);
		swap(topology_, that.topology_);
	}

	template <typename Real, integer N>
	integer Sphere<Real, N>::n() const
	{
		return position_.n();
	}

	template <typename Real, integer N>
	void Sphere<Real, N>::setPosition(
		const Vector<Real, N>& position)
	{
		PENSURE_OP(position_.n(), ==, position.n());

		position_ = position;
	}

	template <typename Real, integer N>
	const Vector<Real, N>& Sphere<Real, N>::position() const
	{
		return position_;
	}

	template <typename Real, integer N>
	void Sphere<Real, N>::setRadius(const Real& radius)
	{
		radius_ = radius;
		inverseRadius_ = inverse(radius_);
	}

	template <typename Real, integer N>
	const Real& Sphere<Real, N>::radius() const
	{
		return radius_;
	}

	template <typename Real, integer N>
	void Sphere<Real, N>::setTopology(Topology topology)
	{
		topology_ = topology;
	}

	template <typename Real, integer N>
	Topology Sphere<Real, N>::topology() const
	{
		return topology_;
	}

	template <typename Real, integer N>
	const Real& Sphere<Real, N>::inverseRadius() const
	{
		return inverseRadius_;
	}

	template <typename Real, integer N>
	Sphere<Real, N>& Sphere<Real, N>::operator+=(
		const Vector<Real, N>& that)
	{
		position_ += that;
		return *this;
	}

	template <typename Real, integer N>
	Sphere<Real, N>& Sphere<Real, N>::operator-=(
		const Vector<Real, N>& that)
	{
		position_ -= that;
		return *this;
	}

	template <typename Real, integer N>
	Sphere<Real, N>& Sphere<Real, N>::operator*=(
		const Real& that)
	{
		radius_ *= that;
		inverseRadius_ /= that;

		return *this;
	}

	template <typename Real, integer N>
	Sphere<Real, N>& Sphere<Real, N>::operator/=(
		const Real& that)
	{
		radius_ /= that;
		inverseRadius_ *= that;

		return *this;
	}

	template <typename Real, integer N>
	void swap(
		Sphere<Real, N>& left,
		Sphere<Real, N>& right)
	{
		left.swap(right);
	}

}

#endif
