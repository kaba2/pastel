// Description: Minkowski norm
// Documentation: norms.txt

#ifndef PASTELMATH_MINKOWSKI_NORM_H
#define PASTELMATH_MINKOWSKI_NORM_H

#include "pastel/math/norm/norm_concept.h"
#include "pastel/math/distance/minkowski_distance.h"

namespace Pastel
{

	template <typename Real>
	struct Minkowski_Norm
	{
		explicit Minkowski_Norm(const Real& p = 2)
		: p_(p)
		{
		}

		auto operator()(const Real& distance = 0) const
		{
			return Minkowski_Distance<Real>(distance, p_);
		}

		Real p_;
	};

}

#endif
