#ifndef PASTEL_VECTOR_HPP
#define PASTEL_VECTOR_HPP

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <int N, typename Real>
	void swap(Vector<N, Real>& left,
		Vector<N, Real>& right)
	{
		left.swap(right);
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> nullVector()
	{
		BOOST_STATIC_ASSERT(N == Dynamic);

		Vector<N, Real> result(ofDimension(0),
				withAliasing<Real>(0));

		return result.asTemporary();
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> aliasVector(integer dimension,
		PASTEL_NO_DEDUCTION(Real)* data)
	{
		Vector<N, Real> result(ofDimension(dimension),
				withAliasing<Real>(data));

		return result.asTemporary();
	}

	template <int N, typename Real, typename Expression>
	TemporaryVector<N, Real> evaluate(
		const VectorExpression<N, Real, Expression>& that)
	{
		Vector<N, Real> result(that);
		return result.asTemporary();
	}

	template <int N, typename Real>
	Tuple<N, Real>& asTuple(
		Vector<N, Real>& that)
	{
		return that.asTuple();
	}

	template <int N, typename Real>
	const Tuple<N, Real>& asTuple(
		const Vector<N, Real>& that)
	{
		return that.asTuple();
	}

	template <int N, typename Real>
	TemporaryTuple<N, Real>& asTuple(
		TemporaryVector<N, Real>& that)
	{
		return that.asTuple();
	}

	template <int N, typename Real>
	const TemporaryTuple<N, Real>& asTuple(
		const TemporaryVector<N, Real>& that)
	{
		return that.asTuple();
	}

}

#endif
