#ifndef PASTEL_SPHERE_HPP
#define PASTEL_SPHERE_HPP

#include "pastel/geometry/sphere.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/constants.h"

namespace Pastel
{

	template <typename Real, int N>
	Sphere<Real, N>::Sphere()
		: position_(0)
		, radius_(0)
		, inverseRadius_(infinity<Real>())
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
	}

	template <typename Real, int N>
	Sphere<Real, N>::Sphere(integer dimension)
		: position_(ofDimension(dimension), 0)
		, radius_(0)
		, inverseRadius_(infinity<Real>())
	{
	}

	template <typename Real, int N>
	Sphere<Real, N>::Sphere(
		const Vector<Real, N>& position,
		const Real& radius)
		: position_(position)
		, radius_(radius)
		, inverseRadius_(inverse(radius))
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
	}

	template <typename Real, int N>
	Sphere<Real, N>::Sphere(
		integer dimension,
		const Vector<Real, N>& position,
		const Real& radius)
		: position_(position)
		, radius_(radius)
		, inverseRadius_(inverse(radius))
	{
		PENSURE_OP(dimension, ==, position.dimension());
	}

	template <typename Real, int N>
	Sphere<Real, N>::~Sphere()
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <typename Real, int N>
	void Sphere<Real, N>::swap(Sphere<Real, N>& that)
	{
		using std::swap;
		using std::swap;

		position_.swap(that.position_);
		swap(radius_, that.radius_);
		swap(inverseRadius_, that.inverseRadius_);
	}

	template <typename Real, int N>
	integer Sphere<Real, N>::dimension() const
	{
		return position_.dimension();
	}

	template <typename Real, int N>
	void Sphere<Real, N>::setPosition(
		const Vector<Real, N>& position)
	{
		PENSURE_OP(position_.dimension(), ==, position.dimension());

		position_ = position;
	}

	template <typename Real, int N>
	const Vector<Real, N>& Sphere<Real, N>::position() const
	{
		return position_;
	}

	template <typename Real, int N>
	void Sphere<Real, N>::setRadius(const Real& radius)
	{
		radius_ = radius;
		inverseRadius_ = inverse(radius_);
	}

	template <typename Real, int N>
	const Real& Sphere<Real, N>::radius() const
	{
		return radius_;
	}

	template <typename Real, int N>
	const Real& Sphere<Real, N>::inverseRadius() const
	{
		return inverseRadius_;
	}

	template <typename Real, int N>
	void swap(Sphere<Real, N>& left,
		Sphere<Real, N>& right)
	{
		left.swap(right);
	}

	template <typename Real, int N>
	Sphere<Real, N>& Sphere<Real, N>::operator+=(
		const Vector<Real, N>& that)
	{
		position_ += that;
		return *this;
	}

	template <typename Real, int N>
	Sphere<Real, N>& Sphere<Real, N>::operator-=(
		const Vector<Real, N>& that)
	{
		position_ -= that;
		return *this;
	}

	template <typename Real, int N>
	Sphere<Real, N>& Sphere<Real, N>::operator*=(
		const Real& that)
	{
		radius_ *= that;
		inverseRadius_ /= that;

		return *this;
	}

	template <typename Real, int N>
	Sphere<Real, N>& Sphere<Real, N>::operator/=(
		const Real& that)
	{
		radius_ /= that;
		inverseRadius_ *= that;

		return *this;
	}

}

#endif
