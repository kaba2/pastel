#ifndef PASTELSYS_RECTANGLEBASE_HPP
#define PASTELSYS_RECTANGLEBASE_HPP

#include "pastel/sys/rectanglebase.h"
#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

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
		const Point<N, integer>& that)
		: min_(that)
		, max_(that)
	{
	}

	template <int N, typename Derived>
	RectangleBase<N, Derived>::RectangleBase(
		const Point<N, integer>& min,
		const Point<N, integer>& max)
		: min_(min)
		, max_(max)
	{
	}

	template <int N, typename Derived>
	RectangleBase<N, Derived>::~RectangleBase()
	{
		BOOST_STATIC_ASSERT(N > 0);
	}

	template <int N, typename Derived>
	bool RectangleBase<N, Derived>::empty() const
	{
		return anyGreaterEqual(min_, max_);
	}

	template <int N, typename Derived>
	void RectangleBase<N, Derived>::swap(Derived& that)
	{
		using NameLookUpTrick::swap;

		swap(min_, that.min_);
		swap(max_, that.max_);
	}

	template <int N, typename Derived>
	void RectangleBase<N, Derived>::set(
		const Point<N, integer>& min,
		const Point<N, integer>& max)
	{
		min_ = min;
		max_ = max;
	}

	template <int N, typename Derived>
	void RectangleBase<N, Derived>::setMin(
		const Point<N, integer>& point)
	{
		min_ = point;
	}

	template <int N, typename Derived>
	Point<N, integer>& RectangleBase<N, Derived>::min()
	{
		return min_;
	}

	template <int N, typename Derived>
	const Point<N, integer>& RectangleBase<N, Derived>::min() const
	{
		return min_;
	}

	template <int N, typename Derived>
	void RectangleBase<N, Derived>::setMax(
		const Point<N, integer>& point)
	{
		max_ = point;
	}

	template <int N, typename Derived>
	Point<N, integer>& RectangleBase<N, Derived>::max()
	{
		return max_;
	}

	template <int N, typename Derived>
	const Point<N, integer>& RectangleBase<N, Derived>::max() const
	{
		return max_;
	}

	template <int N, typename Derived>
	Vector<N, integer> RectangleBase<N, Derived>::extent() const
	{
		return max_ - min_;
	}

	template <int N, typename Derived>
	Point<N, integer> RectangleBase<N, Derived>::at(
		const Vector<N, integer>& coordinates) const
	{
		return Point<N, integer>((1 - coordinates) * asVector(min_) + 
			coordinates * asVector(max_));
	}

	template <int N, typename Derived>
	Derived& RectangleBase<N, Derived>::operator+=(
		const Vector<N, integer>& right)
	{
		min_ += right;
		max_ += right;

		return (Derived&)*this;
	}

	template <int N, typename Derived>
	Derived RectangleBase<N, Derived>::operator+(
		const Vector<N, integer>& right) const
	{
		Derived result((Derived&)*this);
		result += right;
		return result;
	}

	template <int N, typename Derived>
	Derived& RectangleBase<N, Derived>::operator-=(
		const Vector<N, integer>& right)
	{
		min_ -= right;
		max_ -= right;

		return (Derived&)*this;
	}

	template <int N, typename Derived>
	Derived RectangleBase<N, Derived>::operator-(
		const Vector<N, integer>& right) const
	{
		Derived result((Derived&)*this);
		result -= right;
		return result;
	}

}

#endif
