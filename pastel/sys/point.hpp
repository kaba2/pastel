#ifndef PASTEL_POINT_HPP
#define PASTEL_POINT_HPP

#include "pastel/sys/point.h"

namespace Pastel
{

	template <int N, typename Real>
	void swap(Point<N, Real>& left,
		Point<N, Real>& right)
	{
		std::swap(left, right);
	}

	template <int N, typename Real, typename Expression>
	TemporaryPoint<N, Real> asPoint(
		const VectorExpression<N, Real, Expression>& that)
	{
		return TemporaryPoint<N, Real>(that);
	}

	template <int N, typename Real>
	Vector<N, Real>& asVector(
		Point<N, Real>& that)
	{
		return that.asVector();
	}

	template <int N, typename Real>
	const Vector<N, Real>& asVector(
		const Point<N, Real>& that)
	{
		return that.asVector();
	}

	template <int N, typename Real>
	TemporaryVector<N, Real>& asVector(
		TemporaryPoint<N, Real>& that)
	{
		return that.asVector();
	}

	template <int N, typename Real>
	const TemporaryVector<N, Real>& asVector(
		const TemporaryPoint<N, Real>& that)
	{
		return that.asVector();
	}

	template <int N, typename Real>
	Tuple<N, Real>& asTuple(
		Point<N, Real>& that)
	{
		return that.asTuple();
	}

	template <int N, typename Real>
	const Tuple<N, Real>& asTuple(
		const Point<N, Real>& that)
	{
		return that.asTuple();
	}

	template <int N, typename Real>
	TemporaryTuple<N, Real>& asTuple(
		TemporaryPoint<N, Real>& that)
	{
		return that.asTuple();
	}

	template <int N, typename Real>
	const TemporaryTuple<N, Real>& asTuple(
		const TemporaryPoint<N, Real>& that)
	{
		return that.asTuple();
	}

}

#endif
