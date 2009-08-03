#ifndef PASTEL_RAY_HPP
#define PASTEL_RAY_HPP

#include "pastel/geometry/ray.h"

#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

#include "pastel/sys/vector_tools.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <int N, typename Real>
	Ray<N, Real>::Ray()
		: line_()
	{
	}

	template <int N, typename Real>
	Ray<N, Real>::Ray(
		const Point<Real, N>& position,
		const Vector<Real, N>& unitDirection)
		: line_(position, unitDirection)
	{
	}

	template <int N, typename Real>
	Ray<N, Real>::~Ray()
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <int N, typename Real>
	void Ray<N, Real>::swap(Ray<N, Real>& that)
	{
		line_.swap(that.line_);
	}

	template <int N, typename Real>
	Ray<N, Real>& Ray<N, Real>::operator+=(
		const Vector<Real, N>& translation)
	{
		line_ += translation;
		return *this;
	}

	template <int N, typename Real>
	Ray<N, Real> Ray<N, Real>::operator+(
		const Vector<Real, N>& translation) const
	{
		Ray<N, Real> result(*this);
		result += translation;
		return result;
	}

	template <int N, typename Real>
	Ray<N, Real>& Ray<N, Real>::operator-=(
		const Vector<Real, N>& translation)
	{
		line_ -= translation;
		return *this;
	}

	template <int N, typename Real>
	Ray<N, Real> Ray<N, Real>::operator-(
		const Vector<Real, N>& translation) const
	{
		Ray<N, Real> result(*this);
		result -= translation;
		return result;
	}

	template <int N, typename Real>
	void Ray<N, Real>::set(
		const Point<Real, N>& position,
		const Vector<Real, N>& unitDirection)
	{
		line_.set(position, unitDirection);
	}

	template <int N, typename Real>
	void Ray<N, Real>::setPosition(
		const Point<Real, N>& position)
	{
		line_.setPosition(position);
	}

	template <int N, typename Real>
	const Point<Real, N>& Ray<N, Real>::position() const
	{
		return line_.position();
	}

	template <int N, typename Real>
	void Ray<N, Real>::setDirection(
		const Vector<Real, N>& unitDirection)
	{
		line_.setDirection(unitDirection);
	}

	template <int N, typename Real>
	const Vector<Real, N>& Ray<N, Real>::direction() const
	{
		return line_.direction();
	}

	template <int N, typename Real>
	const Vector<Real, N>& Ray<N, Real>::inverseDirection() const
	{
		return line_.inverseDirection();
	}

	template <int N, typename Real>
	Line<N, Real>& Ray<N, Real>::line()
	{
		return line_;
	}

	template <int N, typename Real>
	const Line<N, Real>& Ray<N, Real>::line() const
	{
		return line_;
	}

	template <int N, typename Real>
	Point<Real, N> Ray<N, Real>::at(const Real& t) const
	{
		return line_.at(t);
	}

}

#endif
