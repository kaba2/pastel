#ifndef PASTEL_TUPLE_HPP
#define PASTEL_TUPLE_HPP

#include "pastel/sys/tuple.h"

namespace Pastel
{

	template <int N, typename Real>
	TemporaryTuple<Real, N> nullTuple()
	{
		BOOST_STATIC_ASSERT(N == Dynamic);

		Tuple<Real, N> result(ofDimension(0),
				withAliasing<Real>(0));

		return result.asTemporary();
	}

	template <int N, typename Real>
	TemporaryTuple<Real, N> aliasTuple(integer dimension,
		PASTEL_NO_DEDUCTION(Real)* data)
	{
		Tuple<Real, N> result(ofDimension(dimension),
				withAliasing<Real>(data));

		return result.asTemporary();
	}

}

#endif
