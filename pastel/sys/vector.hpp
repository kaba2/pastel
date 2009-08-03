#ifndef PASTEL_VECTOR_HPP
#define PASTEL_VECTOR_HPP

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <int N, typename Real>
	void swap(Vector<Real, N>& left,
		Vector<Real, N>& right)
	{
		left.swap(right);
	}

	template <int N, typename Real>
	TemporaryVector<Real, N> nullVector()
	{
		BOOST_STATIC_ASSERT(N == Dynamic);

		Vector<Real, N> result(ofDimension(0),
				withAliasing<Real>(0));

		return result.asTemporary();
	}

	template <int N, typename Real>
	TemporaryVector<Real, N> aliasVector(integer dimension,
		PASTEL_NO_DEDUCTION(Real)* data)
	{
		Vector<Real, N> result(ofDimension(dimension),
				withAliasing<Real>(data));

		return result.asTemporary();
	}

	template <int N, typename Real, typename Expression>
	TemporaryVector<Real, N> evaluate(
		const VectorExpression<Real, N, Expression>& that)
	{
		Vector<Real, N> result(that);
		return result.asTemporary();
	}

	template <int N, typename Real>
	Tuple<Real, N>& asTuple(
		Vector<Real, N>& that)
	{
		return that.asTuple();
	}

	template <int N, typename Real>
	const Tuple<Real, N>& asTuple(
		const Vector<Real, N>& that)
	{
		return that.asTuple();
	}

	template <int N, typename Real>
	TemporaryTuple<Real, N>& asTuple(
		TemporaryVector<Real, N>& that)
	{
		return that.asTuple();
	}

	template <int N, typename Real>
	const TemporaryTuple<Real, N>& asTuple(
		const TemporaryVector<Real, N>& that)
	{
		return that.asTuple();
	}

}

#endif
