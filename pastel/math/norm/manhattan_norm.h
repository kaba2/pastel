// Description: Manhattan norm
// Documentation: norms.txt

#ifndef PASTELMATH_MANHATTAN_NORM_H
#define PASTELMATH_MANHATTAN_NORM_H

#include "pastel/math/norm/norm_concept.h"
#include "pastel/math/distance/manhattan_distance.h"

namespace Pastel
{

	template <typename Real>
	struct Manhattan_Norm
	{
		auto operator()(const Real& distance = 0) const
		{
			return Manhattan_Distance<Real>(distance);
		}

		auto operator[](const Real& distance) const
		{
			return Manhattan_Distance<Real>(Distance_Native(), distance);
		}
	};

}

#endif
