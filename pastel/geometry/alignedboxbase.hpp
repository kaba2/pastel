#ifndef PASTELGEOMETRY_ALIGNEDBOXBASE_HPP
#define PASTELGEOMETRY_ALIGNEDBOXBASE_HPP

#include "pastel/geometry/alignedboxbase.h"
#include "pastel/sys/constants.h"

#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <int N, typename Real, typename Derived>
	AlignedBoxBase<N, Real, Derived>::AlignedBoxBase()
		: min_(infinity<Real>())
		, max_(-infinity<Real>())
	{
	}

	template <int N, typename Real, typename Derived>
	AlignedBoxBase<N, Real, Derived>::AlignedBoxBase(
		const Point<N, Real>& that)
		: min_(that)
		, max_(that)
	{
	}

	template <int N, typename Real, typename Derived>
	AlignedBoxBase<N, Real, Derived>::AlignedBoxBase(
		const Point<N, Real>& min,
		const Point<N, Real>& max)
		: min_(min)
		, max_(max)
	{
	}

	template <int N, typename Real, typename Derived>
	AlignedBoxBase<N, Real, Derived>::~AlignedBoxBase()
	{
		BOOST_STATIC_ASSERT(N > 0);
	}

	template <int N, typename Real, typename Derived>
	bool AlignedBoxBase<N, Real, Derived>::empty() const
	{
		return anyGreaterEqual(min_, max_);
	}

	template <int N, typename Real, typename Derived>
	void AlignedBoxBase<N, Real, Derived>::swap(Derived& that)
	{
		using NameLookUpTrick::swap;

		swap(min_, that.min_);
		swap(max_, that.max_);
	}

	template <int N, typename Real, typename Derived>
	void AlignedBoxBase<N, Real, Derived>::set(
		const Point<N, Real>& min,
		const Point<N, Real>& max)
	{
		min_ = min;
		max_ = max;
	}

	template <int N, typename Real, typename Derived>
	void AlignedBoxBase<N, Real, Derived>::setMin(
		const Point<N, Real>& point)
	{
		min_ = point;
	}

	template <int N, typename Real, typename Derived>
	Point<N, Real>& AlignedBoxBase<N, Real, Derived>::min()
	{
		return min_;
	}

	template <int N, typename Real, typename Derived>
	const Point<N, Real>& AlignedBoxBase<N, Real, Derived>::min() const
	{
		return min_;
	}

	template <int N, typename Real, typename Derived>
	void AlignedBoxBase<N, Real, Derived>::setMax(
		const Point<N, Real>& point)
	{
		max_ = point;
	}

	template <int N, typename Real, typename Derived>
	Point<N, Real>& AlignedBoxBase<N, Real, Derived>::max()
	{
		return max_;
	}

	template <int N, typename Real, typename Derived>
	const Point<N, Real>& AlignedBoxBase<N, Real, Derived>::max() const
	{
		return max_;
	}

	template <int N, typename Real, typename Derived>
	Vector<N, Real> AlignedBoxBase<N, Real, Derived>::extent() const
	{
		return max_ - min_;
	}

	template <int N, typename Real, typename Derived>
	Point<N, Real> AlignedBoxBase<N, Real, Derived>::at(
		const Vector<N, Real>& coordinates) const
	{
		return Point<N, Real>((1 - coordinates) * asVector(min_) + 
			coordinates * asVector(max_));
	}

	template <int N, typename Real, typename Derived>
	Derived& AlignedBoxBase<N, Real, Derived>::operator+=(
		const Vector<N, Real>& right)
	{
		min_ += right;
		max_ += right;

		return (Derived&)*this;
	}

	template <int N, typename Real, typename Derived>
	Derived AlignedBoxBase<N, Real, Derived>::operator+(
		const Vector<N, Real>& right) const
	{
		Derived result((Derived&)*this);
		result += right;
		return result;
	}

	template <int N, typename Real, typename Derived>
	Derived& AlignedBoxBase<N, Real, Derived>::operator-=(
		const Vector<N, Real>& right)
	{
		min_ -= right;
		max_ -= right;

		return (Derived&)*this;
	}

	template <int N, typename Real, typename Derived>
	Derived AlignedBoxBase<N, Real, Derived>::operator-(
		const Vector<N, Real>& right) const
	{
		Derived result((Derived&)*this);
		result -= right;
		return result;
	}

}

#endif
