#ifndef PASTEL_POINT_HPP
#define PASTEL_POINT_HPP

#include "pastel/sys/point.h"

namespace Pastel
{

	template <int N, typename Real>
	void swap(Point<Real, N>& left,
		Point<Real, N>& right)
	{
		std::swap(left, right);
	}

	template <int N, typename Real, typename Expression>
	Point<Real, N> asPoint(
		const VectorExpression<Real, N, Expression>& that)
	{
		return Point<Real, N>(that);
	}

	template <int N, typename Real>
	Vector<Real, N>& asVector(
		Point<Real, N>& that)
	{
		return that.asVector();
	}

	template <int N, typename Real>
	const Vector<Real, N>& asVector(
		const Point<Real, N>& that)
	{
		return that.asVector();
	}

	template <int N, typename Real>
	Tuple<Real, N>& asTuple(
		Point<Real, N>& that)
	{
		return that.asTuple();
	}

	template <int N, typename Real>
	const Tuple<Real, N>& asTuple(
		const Point<Real, N>& that)
	{
		return that.asTuple();
	}

}

#endif
