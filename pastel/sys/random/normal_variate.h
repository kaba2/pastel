// Description: Variate from normal distribution

#ifndef PASTELSYS_NORMAL_VARIATE_H
#define PASTELSYS_NORMAL_VARIATE_H

#include "pastel/sys/random/normal_distribution.h"

namespace Pastel
{

	template <typename Real>
	arma::Col<Real> random(
		const Normal_Distribution<Real>& distribution)
	{
		using Point = typename Normal_Distribution<Real>::Point;

		Point result(distribution.n());
		result.for_each([](Real& x){x = randomGaussian<Real>();});

		return distribution.rotation() * 
			(distribution.scale() % result);
	}

}

#endif
