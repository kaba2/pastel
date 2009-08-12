#ifndef PASTEL_PROJECTION_HPP
#define PASTEL_PROJECTION_HPP

#include "pastel/geometry/projection.h"

namespace Pastel
{

	template <int N, typename Real>
	Point<Real, N> wDivide(const Point<Real, N>& that)
	{
		const integer dimension = that.dimension();

		Point<Real, N> result(that);
		const Real& wInv = inverse(that[dimension - 1]);

		for (integer i = 0;i < dimension - 1;++i)
		{
			result[i] *= wInv;
		}

		return result;
	}

}

#endif
