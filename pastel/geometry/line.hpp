#ifndef PASTEL_LINE_HPP
#define PASTEL_LINE_HPP

#include "pastel/geometry/line.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Line<Real, N>::Line()
		: position_(0)
		, direction_(unitAxis<Real, N>(0))
		, inverseDirection_(inverse(direction_))
	{
		PASTEL_STATIC_ASSERT(N != Dynamic);
	}

	template <typename Real, int N>
	Line<Real, N>::Line(integer dimension)
		: position_(ofDimension(dimension), 0)
		, direction_(unitAxis<Real, N>(dimension, 0))
		, inverseDirection_(inverse(direction_))
	{
	}

	template <typename Real, int N>
	Line<Real, N>::Line(
		const Vector<Real, N>& position,
		const Vector<Real, N>& unitDirection)
		: position_(position)
		, direction_(unitDirection)
		, inverseDirection_(inverse(unitDirection))
	{
		PASTEL_STATIC_ASSERT(N != Dynamic);
	}

	template <typename Real, int N>
	Line<Real, N>::Line(
		integer dimension,
		const Vector<Real, N>& position,
		const Vector<Real, N>& unitDirection)
		: position_(position)
		, direction_(unitDirection)
		, inverseDirection_(inverse(unitDirection))
	{
		PENSURE_OP(dimension, ==, position.dimension());
		PENSURE_OP(dimension, ==, unitDirection.dimension());
	}

	template <typename Real, int N>
	Line<Real, N>::~Line()
	{
		PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <typename Real, int N>
	void Line<Real, N>::swap(Line<Real, N>& that)
	{
		using std::swap;
		using std::swap;

		swap(position_, that.position_);
		swap(direction_, that.direction_);
		swap(inverseDirection_, that.inverseDirection_);
	}

	template <typename Real, int N>
	integer Line<Real, N>::dimension() const
	{
		return position_.dimension();
	}

	template <typename Real, int N>
	void Line<Real, N>::set(
		const Vector<Real, N>& position,
		const Vector<Real, N>& unitDirection)
	{
		setPosition(position);
		setDirection(unitDirection);
	}

	template <typename Real, int N>
	void Line<Real, N>::setPosition(
		const Vector<Real, N>& position)
	{
		PENSURE_OP(position_.dimension(), ==, position.dimension());

		position_ = position;
	}

	template <typename Real, int N>
	const Vector<Real, N>& Line<Real, N>::position() const
	{
		return position_;
	}

	template <typename Real, int N>
	void Line<Real, N>::setDirection(
		const Vector<Real, N>& unitDirection)
	{
		PENSURE_OP(direction_.dimension(), ==, unitDirection.dimension());

		direction_ = unitDirection;

		inverseDirection_ =
			inverse(unitDirection);
	}

	template <typename Real, int N>
	const Vector<Real, N>& Line<Real, N>::direction() const
	{
		return direction_;
	}

	template <typename Real, int N>
	const Vector<Real, N>& Line<Real, N>::inverseDirection() const
	{
		return inverseDirection_;
	}

	template <typename Real, int N>
	Vector<Real, N> Line<Real, N>::at(const Real& t) const
	{
		return position_ + direction_ * t;
	}

	template <typename Real, int N>
	Line<Real, N>& Line<Real, N>::operator+=(
		const Vector<Real, N>& that)
	{
		position_ += that;

		return *this;
	}

	template <typename Real, int N>
	Line<Real, N>& Line<Real, N>::operator-=(
		const Vector<Real, N>& that)
	{
		position_ -= that;

		return *this;
	}

	template <typename Real, int N>
	Line<Real, N>& Line<Real, N>::operator*=(
		const Real& that)
	{
		// Do nothing.
		
		unused(that);

		return *this;
	}

	template <typename Real, int N>
	Line<Real, N>& Line<Real, N>::operator/=(
		const Real& that)
	{
		PENSURE_OP(that, !=, 0);

		// Do nothing.

		return *this;
	}

}

#endif
