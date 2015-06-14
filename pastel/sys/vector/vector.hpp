#ifndef PASTELSYS_VECTOR_HPP
#define PASTELSYS_VECTOR_HPP

#include "pastel/sys/vector.h"
#include "pastel/sys/hashing.h"

namespace Pastel
{

	template <typename Real, integer N>
	inline void swap(Vector<Real, N>& left,
		Vector<Real, N>& right)
	{
		left.swap(right);
	}

	template <typename Real, integer N, typename Expression>
	Vector<Real, N> evaluate(
		const VectorExpression<Real, N, Expression>& that)
	{
		Vector<Real, N> result(that);
		return result;
	}

	template <typename Real, integer N>
	Tuple<Real, N>& asTuple(
		Vector<Real, N>& that)
	{
		return that.asTuple();
	}

	template <typename Real, integer N>
	const Tuple<Real, N>& asTuple(
		const Vector<Real, N>& that)
	{
		return that.asTuple();
	}

}

#endif
