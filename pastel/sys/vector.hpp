#ifndef PASTELSYS_VECTOR_HPP
#define PASTELSYS_VECTOR_HPP

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <int N, typename Real>
	void swap(Vector<N, Real>& left,
		Vector<N, Real>& right)
	{
		left.swap(right);
	}

	template <int N, typename Real, typename Expression>
	TemporaryVector<N, Real> evaluate(
		const VectorExpression<N, Real, Expression>& that)
	{
		Vector<N, Real> result(that);
		return result.asTemporary();
	}

	template <int N, typename Real>
	Tuple<N, Real> asTuple(Vector<N, Real>& that)
	{
		return that.asTuple();
	}

	template <int N, typename Real>
	const Tuple<N, Real> asTuple(const Vector<N, Real>& that)
	{
		return that.asTuple();
	}

}

#endif
