#ifndef PASTEL_RAY_HPP
#define PASTEL_RAY_HPP

#include "pastel/geometry/ray.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <typename Real, int N>
	Ray<Real, N>::Ray()
		: line_()
	{
	}

	template <typename Real, int N>
	Ray<Real, N>::Ray(
		const Vector<Real, N>& position,
		const Vector<Real, N>& unitDirection)
		: line_(position, unitDirection)
	{
	}

	template <typename Real, int N>
	Ray<Real, N>::~Ray()
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <typename Real, int N>
	void Ray<Real, N>::swap(Ray<Real, N>& that)
	{
		line_.swap(that.line_);
	}

	template <typename Real, int N>
	Ray<Real, N>& Ray<Real, N>::operator+=(
		const Vector<Real, N>& translation)
	{
		line_ += translation;
		return *this;
	}

	template <typename Real, int N>
	Ray<Real, N> Ray<Real, N>::operator+(
		const Vector<Real, N>& translation) const
	{
		Ray<Real, N> result(*this);
		result += translation;
		return result;
	}

	template <typename Real, int N>
	Ray<Real, N>& Ray<Real, N>::operator-=(
		const Vector<Real, N>& translation)
	{
		line_ -= translation;
		return *this;
	}

	template <typename Real, int N>
	Ray<Real, N> Ray<Real, N>::operator-(
		const Vector<Real, N>& translation) const
	{
		Ray<Real, N> result(*this);
		result -= translation;
		return result;
	}

	template <typename Real, int N>
	void Ray<Real, N>::set(
		const Vector<Real, N>& position,
		const Vector<Real, N>& unitDirection)
	{
		line_.set(position, unitDirection);
	}

	template <typename Real, int N>
	void Ray<Real, N>::setPosition(
		const Vector<Real, N>& position)
	{
		line_.setPosition(position);
	}

	template <typename Real, int N>
	const Vector<Real, N>& Ray<Real, N>::position() const
	{
		return line_.position();
	}

	template <typename Real, int N>
	void Ray<Real, N>::setDirection(
		const Vector<Real, N>& unitDirection)
	{
		line_.setDirection(unitDirection);
	}

	template <typename Real, int N>
	const Vector<Real, N>& Ray<Real, N>::direction() const
	{
		return line_.direction();
	}

	template <typename Real, int N>
	const Vector<Real, N>& Ray<Real, N>::inverseDirection() const
	{
		return line_.inverseDirection();
	}

	template <typename Real, int N>
	Line<Real, N>& Ray<Real, N>::line()
	{
		return line_;
	}

	template <typename Real, int N>
	const Line<Real, N>& Ray<Real, N>::line() const
	{
		return line_;
	}

	template <typename Real, int N>
	Vector<Real, N> Ray<Real, N>::at(const Real& t) const
	{
		return line_.at(t);
	}

}

#endif
