// Description: Variate from normal distribution

#ifndef PASTELSYS_NORMAL_VARIATE_H
#define PASTELSYS_NORMAL_VARIATE_H

#include "pastel/sys/random/normal_distribution.h"

namespace Pastel
{

	template <typename Real>
	ColMatrix<Real> random(
		const Normal_Distribution<Real>& distribution)
	{
		auto x = randomGaussianVector<Real>(distribution.n());

		return distribution.rotation() * 
			(distribution.scale().array() * asColumnMatrix(x).array()).matrix();
	}

}

#endif
