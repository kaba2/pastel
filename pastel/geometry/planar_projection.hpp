#ifndef PASTEL_PLANAR_PROJECTION_HPP
#define PASTEL_PLANAR_PROJECTION_HPP

#include "pastel/geometry/planar_projection.h"

namespace Pastel
{

	template <typename Real, int N>
	Vector<Real, N> wDivide(const Vector<Real, N>& that)
	{
		const integer dimension = that.dimension();

		Vector<Real, N> result(that);
		const Real& wInv = inverse(that[dimension - 1]);

		for (integer i = 0;i < dimension - 1;++i)
		{
			result[i] *= wInv;
		}

		return result;
	}

}

#endif
