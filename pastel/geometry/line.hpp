#ifndef PASTEL_LINE_HPP
#define PASTEL_LINE_HPP

#include "pastel/geometry/line.h"

#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

#include "pastel/sys/vector_tools.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <int N, typename Real>
	Line<N, Real>::Line()
		: position_(0)
		, direction_(unitAxis<Real, N>(0))
		, inverseDirection_(inverse(direction_))
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
	}

	template <int N, typename Real>
	Line<N, Real>::Line(integer dimension)
		: position_(ofDimension(dimension), 0)
		, direction_(unitAxis<Real, N>(dimension, 0))
		, inverseDirection_(inverse(direction_))
	{
	}

	template <int N, typename Real>
	Line<N, Real>::Line(
		const Point<Real, N>& position,
		const Vector<Real, N>& unitDirection)
		: position_(position)
		, direction_(unitDirection)
		, inverseDirection_(inverse(unitDirection))
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
	}

	template <int N, typename Real>
	Line<N, Real>::Line(
		integer dimension,
		const Point<Real, N>& position,
		const Vector<Real, N>& unitDirection)
		: position_(position)
		, direction_(unitDirection)
		, inverseDirection_(inverse(unitDirection))
	{
		PENSURE_OP(dimension, ==, position.dimension());
		PENSURE_OP(dimension, ==, unitDirection.dimension());
	}

	template <int N, typename Real>
	Line<N, Real>::~Line()
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <int N, typename Real>
	void Line<N, Real>::swap(Line<N, Real>& that)
	{
		using std::swap;
		using std::swap;

		swap(position_, that.position_);
		swap(direction_, that.direction_);
		swap(inverseDirection_, that.inverseDirection_);
	}

	template <int N, typename Real>
	Line<N, Real>& Line<N, Real>::operator+=(
		const Vector<Real, N>& translation)
	{
		position_ += translation;
		return *this;
	}

	template <int N, typename Real>
	Line<N, Real> Line<N, Real>::operator+(
		const Vector<Real, N>& translation) const
	{
		Line<N, Real> result(*this);
		result += translation;
		return result;
	}

	template <int N, typename Real>
	Line<N, Real>& Line<N, Real>::operator-=(
		const Vector<Real, N>& translation)
	{
		position_ -= translation;
		return *this;
	}

	template <int N, typename Real>
	Line<N, Real> Line<N, Real>::operator-(
		const Vector<Real, N>& translation) const
	{
		Line<N, Real> result(*this);
		result -= translation;
		return result;
	}

	template <int N, typename Real>
	integer Line<N, Real>::dimension() const
	{
		return position_.dimension();
	}

	template <int N, typename Real>
	void Line<N, Real>::set(
		const Point<Real, N>& position,
		const Vector<Real, N>& unitDirection)
	{
		setPosition(position);
		setDirection(unitDirection);
	}

	template <int N, typename Real>
	void Line<N, Real>::setPosition(
		const Point<Real, N>& position)
	{
		PENSURE_OP(position_.dimension(), ==, position.dimension());

		position_ = position;
	}

	template <int N, typename Real>
	const Point<Real, N>& Line<N, Real>::position() const
	{
		return position_;
	}

	template <int N, typename Real>
	void Line<N, Real>::setDirection(
		const Vector<Real, N>& unitDirection)
	{
		PENSURE_OP(direction_.dimension(), ==, unitDirection.dimension());

		direction_ = unitDirection;

		inverseDirection_ =
			inverse(unitDirection);
	}

	template <int N, typename Real>
	const Vector<Real, N>& Line<N, Real>::direction() const
	{
		return direction_;
	}

	template <int N, typename Real>
	const Vector<Real, N>& Line<N, Real>::inverseDirection() const
	{
		return inverseDirection_;
	}

	template <int N, typename Real>
	Point<Real, N> Line<N, Real>::at(const Real& t) const
	{
		return position_ + direction_ * t;
	}

}

#endif
