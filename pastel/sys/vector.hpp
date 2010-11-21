#ifndef PASTEL_VECTOR_HPP
#define PASTEL_VECTOR_HPP

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real, int N>
	inline void swap(Vector<Real, N>& left,
		Vector<Real, N>& right)
	{
		left.swap(right);
	}

	template <typename Real, int N, typename HashFunction>
	inline uint32 partialHash(
		const Vector<Real, N>& that,
		uint32 currentHash,
		const HashFunction& hashFunction)
	{
		return Pastel::partialHashMany(
			forwardRange(that.begin(), that.end()),
			currentHash, hashFunction);
	}

	template <typename Real, int N, typename Expression>
	Vector<Real, N> evaluate(
		const VectorExpression<Real, N, Expression>& that)
	{
		Vector<Real, N> result(that);
		return result;
	}

	template <typename Real, int N>
	Tuple<Real, N>& asTuple(
		Vector<Real, N>& that)
	{
		return that.asTuple();
	}

	template <typename Real, int N>
	const Tuple<Real, N>& asTuple(
		const Vector<Real, N>& that)
	{
		return that.asTuple();
	}

}

#endif
