// Description: Euclidean norm
// Documentation: norms.txt

#ifndef PASTELMATH_EUCLIDEAN_NORM_H
#define PASTELMATH_EUCLIDEAN_NORM_H

#include "pastel/math/norm/norm_concept.h"
#include "pastel/math/distance/euclidean_distance.h"

namespace Pastel
{

	template <typename Real>
	struct Euclidean_Norm
	{
		auto operator()(const Real& distance = 0) const
		{
			return Euclidean_Distance<Real>(distance);
		}

		auto operator[](const Real& distance) const
		{
			return Euclidean_Distance<Real>(Distance_Native(), distance);
		}
	};

}

#endif
