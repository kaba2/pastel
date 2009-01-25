#ifndef PASTELGEOMETRY_SPHERE_HPP
#define PASTELGEOMETRY_SPHERE_HPP

#include "pastel/geometry/sphere.h"
#include "pastel/sys/point.h"

#include "pastel/sys/constants.h"

namespace Pastel
{

	template <int N, typename Real>
	Sphere<N, Real>::Sphere()
		: position_(0)
		, radius_(0)
		, inverseRadius_(infinity<Real>())
	{
	}

	template <int N, typename Real>
	Sphere<N, Real>::Sphere(
		const Point<N, Real>& position,
		const Real& radius)
		: position_(position)
		, radius_(radius)
		, inverseRadius_(inverse(radius))
	{
	}

	template <int N, typename Real>
	Sphere<N, Real>::~Sphere()
	{
		BOOST_STATIC_ASSERT(N > 0);
	}

	template <int N, typename Real>
	void Sphere<N, Real>::swap(Sphere<N, Real>& that)
	{
		using std::swap;
		using NameLookUpTrick::swap;

		position_.swap(that.position_);
		swap(radius_, that.radius_);
		swap(inverseRadius_, that.inverseRadius_);
	}

	template <int N, typename Real>
	void Sphere<N, Real>::setPosition(
		const Point<N, Real>& position)
	{
		position_ = position;
	}

	template <int N, typename Real>
	const Point<N, Real>& Sphere<N, Real>::position() const
	{
		return position_;
	}

	template <int N, typename Real>
	void Sphere<N, Real>::setRadius(const Real& radius)
	{
		radius_ = radius;
		inverseRadius_ = inverse(radius_);
	}

	template <int N, typename Real>
	const Real& Sphere<N, Real>::radius() const
	{
		return radius_;
	}

	template <int N, typename Real>
	const Real& Sphere<N, Real>::inverseRadius() const
	{
		return inverseRadius_;
	}

	template <int N, typename Real>
	void swap(Sphere<N, Real>& left,
		Sphere<N, Real>& right)
	{
		left.swap(right);
	}

}
#endif
