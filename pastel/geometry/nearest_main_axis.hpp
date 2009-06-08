#ifndef PASTEL_NEAREST_MAIN_AXIS_HPP
#define PASTEL_NEAREST_MAIN_AXIS_HPP

#include "pastel/geometry/nearest_main_axis.h"

namespace Pastel
{

	template <int N, typename Real>
	integer nearestMainAxis(
		const Vector<N, Real>& that)
	{
		Real maxProjectedLength = -infinity<Real>();
		integer nearestAxis = 0;

		const integer dimension = that.dimension();
		for (integer i = 0;i < dimension;++i)
		{
			// <e_i, that> / |e_i| = that[i]
			const Real projectedLength = 
				mabs(that[i]);
			
			if (projectedLength > maxProjectedLength)
			{
				nearestAxis = i;
				maxProjectedLength = projectedLength;
			}
		}

		return nearestAxis;
	}

}

#endif
