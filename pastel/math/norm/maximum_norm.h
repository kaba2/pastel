// Description: Maximum norm
// Documentation: norms.txt

#ifndef PASTELMATH_MAXIMUM_NORM_H
#define PASTELMATH_MAXIMUM_NORM_H

#include "pastel/math/norm/norm_concept.h"
#include "pastel/math/distance/maximum_distance.h"

namespace Pastel
{

	template <typename Real>
	struct Maximum_Norm
	{
		auto operator()(const Real& distance = 0) const
		{
			return Maximum_Distance<Real>(distance);
		}

		auto operator[](const Real& distance) const
		{
			return Maximum_Distance<Real>(Distance_Native(), distance);
		}
	};

}

#endif
