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

	template <int N, typename Real, typename Expression>
	Vector<Real, N> evaluate(
		const VectorExpression<Real, N, Expression>& that)
	{
		Vector<Real, N> result(that);
		return result;
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

}

#endif
