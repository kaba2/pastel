#ifndef PASTELGEOMETRY_PROJECTION_HPP
#define PASTELGEOMETRY_PROJECTION_HPP

#include "pastel/geometry/projection.h"

namespace Pastel
{

	template <int N, typename Real>
	Point<N, Real> wDivide(const Point<N, Real>& that)
	{
		Point<N, Real> result(that);
		const Real& wInv = inverse(that[N - 1]);
		
		for (integer i = 0;i < N - 1;++i)
		{
			result[i] *= wInv;
		}

		return result;
	}

}

#endif
