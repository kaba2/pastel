#ifndef PASTELGEOMETRY_PROJECTION_HPP
#define PASTELGEOMETRY_PROJECTION_HPP

#include "pastel/geometry/projection.h"

namespace Pastel
{

	template <int N, typename Real>
	TemporaryPoint<N, Real> wDivide(const Point<N, Real>& that)
	{
		const integer dimension = that.dimension();

		Point<N, Real> result(that);
		const Real& wInv = inverse(that[dimension - 1]);

		for (integer i = 0;i < dimension - 1;++i)
		{
			result[i] *= wInv;
		}

		return result.asTemporary();
	}

}

#endif
