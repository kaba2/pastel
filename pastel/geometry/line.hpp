#ifndef PASTELGEOMETRY_LINE_HPP
#define PASTELGEOMETRY_LINE_HPP

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
		, direction_(unitAxis<N, Real>(0))
		, inverseDirection_(inverse(direction_))
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
	}

	template <int N, typename Real>
	Line<N, Real>::Line(integer dimension)
		: position_(ofDimension(dimension), 0)
		, direction_(unitAxis<N, Real>(dimension, 0))
		, inverseDirection_(inverse(direction_))
	{
	}

	template <int N, typename Real>
	Line<N, Real>::Line(
		const Point<N, Real>& position,
		const Vector<N, Real>& unitDirection)
		: position_(position)
		, direction_(unitDirection)
		, inverseDirection_(inverse(unitDirection))
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
	}

	template <int N, typename Real>
	Line<N, Real>::Line(
		integer dimension,
		const Point<N, Real>& position,
		const Vector<N, Real>& unitDirection)
		: position_(position)
		, direction_(unitDirection)
		, inverseDirection_(inverse(unitDirection))
	{
		PENSURE2(dimension == position.dimension(),
			dimension, position.dimension());
		PENSURE2(dimension == unitDirection.dimension(),
			dimension, unitDirection.dimension());
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
		const Vector<N, Real>& translation)
	{
		position_ += translation;
		return *this;
	}

	template <int N, typename Real>
	Line<N, Real> Line<N, Real>::operator+(
		const Vector<N, Real>& translation) const
	{
		Line<N, Real> result(*this);
		result += translation;
		return result;
	}

	template <int N, typename Real>
	Line<N, Real>& Line<N, Real>::operator-=(
		const Vector<N, Real>& translation)
	{
		position_ -= translation;
		return *this;
	}

	template <int N, typename Real>
	Line<N, Real> Line<N, Real>::operator-(
		const Vector<N, Real>& translation) const
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
		const Point<N, Real>& position,
		const Vector<N, Real>& unitDirection)
	{
		setPosition(position);
		setDirection(unitDirection);
	}

	template <int N, typename Real>
	void Line<N, Real>::setPosition(
		const Point<N, Real>& position)
	{
		PENSURE2(position_.dimension() == position.dimension(),
			position_.dimension(), position.dimension());

		position_ = position;
	}

	template <int N, typename Real>
	const Point<N, Real>& Line<N, Real>::position() const
	{
		return position_;
	}

	template <int N, typename Real>
	void Line<N, Real>::setDirection(
		const Vector<N, Real>& unitDirection)
	{
		PENSURE2(direction_.dimension() == unitDirection.dimension(),
			direction_.dimension(), unitDirection.dimension());

		direction_ = unitDirection;

		inverseDirection_ =
			inverse(unitDirection);
	}

	template <int N, typename Real>
	const Vector<N, Real>& Line<N, Real>::direction() const
	{
		return direction_;
	}

	template <int N, typename Real>
	const Vector<N, Real>& Line<N, Real>::inverseDirection() const
	{
		return inverseDirection_;
	}

	template <int N, typename Real>
	Point<N, Real> Line<N, Real>::at(const Real& t) const
	{
		return position_ + direction_ * t;
	}

}

#endif
