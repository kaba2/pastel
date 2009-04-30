#ifndef PASTELGEOMETRY_RAY_HPP
#define PASTELGEOMETRY_RAY_HPP

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
		const Point<N, Real>& position,
		const Vector<N, Real>& unitDirection)
		: line_(position, unitDirection)
	{
	}

	template <int N, typename Real>
	Ray<N, Real>::~Ray()
	{
		BOOST_STATIC_ASSERT(N == Unbounded || N > 0);
	}

	template <int N, typename Real>
	void Ray<N, Real>::swap(Ray<N, Real>& that)
	{
		line_.swap(that.line_);
	}

	template <int N, typename Real>
	Ray<N, Real>& Ray<N, Real>::operator+=(
		const Vector<N, Real>& translation)
	{
		line_ += translation;
		return *this;
	}

	template <int N, typename Real>
	Ray<N, Real> Ray<N, Real>::operator+(
		const Vector<N, Real>& translation) const
	{
		Ray<N, Real> result(*this);
		result += translation;
		return result;
	}

	template <int N, typename Real>
	Ray<N, Real>& Ray<N, Real>::operator-=(
		const Vector<N, Real>& translation)
	{
		line_ -= translation;
		return *this;
	}

	template <int N, typename Real>
	Ray<N, Real> Ray<N, Real>::operator-(
		const Vector<N, Real>& translation) const
	{
		Ray<N, Real> result(*this);
		result -= translation;
		return result;
	}

	template <int N, typename Real>
	void Ray<N, Real>::set(
		const Point<N, Real>& position,
		const Vector<N, Real>& unitDirection)
	{
		line_.set(position, unitDirection);
	}

	template <int N, typename Real>
	void Ray<N, Real>::setPosition(
		const Point<N, Real>& position)
	{
		line_.setPosition(position);
	}

	template <int N, typename Real>
	const Point<N, Real>& Ray<N, Real>::position() const
	{
		return line_.position();
	}

	template <int N, typename Real>
	void Ray<N, Real>::setDirection(
		const Vector<N, Real>& unitDirection)
	{
		line_.setDirection(unitDirection);
	}

	template <int N, typename Real>
	const Vector<N, Real>& Ray<N, Real>::direction() const
	{
		return line_.direction();
	}

	template <int N, typename Real>
	const Vector<N, Real>& Ray<N, Real>::inverseDirection() const
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
	Point<N, Real> Ray<N, Real>::at(const Real& t) const
	{
		return line_.at(t);
	}

}

#endif
