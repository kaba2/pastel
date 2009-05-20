#ifndef PASTEL_TUPLE_HPP
#define PASTEL_TUPLE_HPP

#include "pastel/sys/tuple.h"

namespace Pastel
{

	template <int N, typename Real>
	TemporaryTuple<N, Real> nullTuple()
	{
		BOOST_STATIC_ASSERT(N == Dynamic);

		Tuple<N, Real> result(ofDimension(0),
				withAliasing<Real>(0));

		return result.asTemporary();
	}

	template <int N, typename Real>
	TemporaryTuple<N, Real> aliasTuple(integer dimension,
		PASTEL_NO_DEDUCTION(Real)* data)
	{
		Tuple<N, Real> result(ofDimension(dimension),
				withAliasing<Real>(data));

		return result.asTemporary();
	}

}

#endif
