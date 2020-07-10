// Description: Moments of normal distribution

#ifndef PASTELSYS_NORMAL_MOMENTS_H
#define PASTELSYS_NORMAL_MOMENTS_H

#include "pastel/sys/random/normal_distribution.h"

#include <cmath>

namespace Pastel
{

	// Mean of normal distribution.
	template <typename Real>
	decltype(auto) mean(const Normal_Distribution<Real>& distribution)
	{
		return distribution.mean();
	}

	// Variance of normal distribution.
	template <typename Real>
	decltype(auto) variance(const Normal_Distribution<Real>& distribution)
	{
		return distribution.rotation() * 
			diagonalMatrix(distribution.scale().array().square().matrix().eval()) * 
			distribution.rotation().transpose();
	}

}

#endif
