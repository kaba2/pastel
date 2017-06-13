// Description: Entropy of normal distribution

#ifndef PASTELSYS_NORMAL_ENTROPY_H
#define PASTELSYS_NORMAL_ENTROPY_H

#include "pastel/sys/random/random_gaussian.h"

#include <cmath>

namespace Pastel
{

	//! Entropy of normal distribution.
	template <typename Real>
	Real entropy(const Normal_Distribution<Real>& distribution)
	{
		return distribution.n() * std::log(
				2 * constantPi<Real>() * constantNeper<Real>()
			) + std::log(covariance(distribution).det());
	}

}

#endif
