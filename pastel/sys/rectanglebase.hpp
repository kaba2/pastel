#ifndef PASTEL_RECTANGLEBASE_HPP
#define PASTEL_RECTANGLEBASE_HPP

#include "pastel/sys/rectanglebase.h"
#include "pastel/sys/point_tools.h"
#include "pastel/sys/vector_tools.h"

#include <boost/static_assert.hpp>

#include <limits>

namespace Pastel
{

	template <int N, typename Derived>
	RectangleBase<N, Derived>::RectangleBase()
		: min_(std::numeric_limits<integer>::max())
		, max_(std::numeric_limits<integer>::min())
	{
	}

	template <int N, typename Derived>
	RectangleBase<N, Derived>::RectangleBase(
		const Point<integer, N>& that)
		: min_(that)
		, max_(that)
	{
	}

	template <int N, typename Derived>
	RectangleBase<N, Derived>::RectangleBase(
		const Point<integer, N>& min,
		const Point<integer, N>& max)
		: min_(min)
		, max_(max)
	{
	}

	template <int N, typename Derived>
	RectangleBase<N, Derived>::~RectangleBase()
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <int N, typename Derived>
	bool RectangleBase<N, Derived>::empty() const
	{
		return anyGreaterEqual(min_, max_);
	}

	template <int N, typename Derived>
	void RectangleBase<N, Derived>::swap(Derived& that)
	{
		using std::swap;

		swap(min_, that.min_);
		swap(max_, that.max_);
	}

	template <int N, typename Derived>
	void RectangleBase<N, Derived>::set(
		const Point<integer, N>& min,
		const Point<integer, N>& max)
	{
		min_ = min;
		max_ = max;
	}

	template <int N, typename Derived>
	void RectangleBase<N, Derived>::setMin(
		const Point<integer, N>& point)
	{
		min_ = point;
	}

	template <int N, typename Derived>
	Point<integer, N>& RectangleBase<N, Derived>::min()
	{
		return min_;
	}

	template <int N, typename Derived>
	const Point<integer, N>& RectangleBase<N, Derived>::min() const
	{
		return min_;
	}

	template <int N, typename Derived>
	void RectangleBase<N, Derived>::setMax(
		const Point<integer, N>& point)
	{
		max_ = point;
	}

	template <int N, typename Derived>
	Point<integer, N>& RectangleBase<N, Derived>::max()
	{
		return max_;
	}

	template <int N, typename Derived>
	const Point<integer, N>& RectangleBase<N, Derived>::max() const
	{
		return max_;
	}

	template <int N, typename Derived>
	Vector<integer, N> RectangleBase<N, Derived>::extent() const
	{
		return max_ - min_;
	}

	template <int N, typename Derived>
	Point<integer, N> RectangleBase<N, Derived>::at(
		const Vector<integer, N>& coordinates) const
	{
		return Point<integer, N>((1 - coordinates) * asVector(min_) +
			coordinates * asVector(max_));
	}

	template <int N, typename Derived>
	Derived& RectangleBase<N, Derived>::operator+=(
		const Vector<integer, N>& right)
	{
		min_ += right;
		max_ += right;

		return (Derived&)*this;
	}

	template <int N, typename Derived>
	Derived RectangleBase<N, Derived>::operator+(
		const Vector<integer, N>& right) const
	{
		Derived result((Derived&)*this);
		result += right;
		return result;
	}

	template <int N, typename Derived>
	Derived& RectangleBase<N, Derived>::operator-=(
		const Vector<integer, N>& right)
	{
		min_ -= right;
		max_ -= right;

		return (Derived&)*this;
	}

	template <int N, typename Derived>
	Derived RectangleBase<N, Derived>::operator-(
		const Vector<integer, N>& right) const
	{
		Derived result((Derived&)*this);
		result -= right;
		return result;
	}

}

#endif
