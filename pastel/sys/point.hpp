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

	template <int N, typename Real>
	TemporaryPoint<Real, N> nullPoint()
	{
		BOOST_STATIC_ASSERT(N == Dynamic);

		Point<Real, N> result(ofDimension(0),
				withAliasing<Real>(0));

		return result.asTemporary();
	}

	template <int N, typename Real>
	TemporaryPoint<Real, N> aliasPoint(integer dimension,
		PASTEL_NO_DEDUCTION(Real)* data)
	{
		Point<Real, N> result(ofDimension(dimension),
				withAliasing<Real>(data));

		return result.asTemporary();
	}

	template <int N, typename Real, typename Expression>
	TemporaryPoint<Real, N> asPoint(
		const VectorExpression<Real, N, Expression>& that)
	{
		return TemporaryPoint<Real, N>(that);
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
	TemporaryVector<Real, N>& asVector(
		TemporaryPoint<Real, N>& that)
	{
		return that.asVector();
	}

	template <int N, typename Real>
	const TemporaryVector<Real, N>& asVector(
		const TemporaryPoint<Real, N>& that)
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

	template <int N, typename Real>
	TemporaryTuple<Real, N>& asTuple(
		TemporaryPoint<Real, N>& that)
	{
		return that.asTuple();
	}

	template <int N, typename Real>
	const TemporaryTuple<Real, N>& asTuple(
		const TemporaryPoint<Real, N>& that)
	{
		return that.asTuple();
	}

}

#endif
