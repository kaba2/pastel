// Description: Pdf of normal distribution

#ifndef PASTELSYS_NORMAL_PDF_H
#define PASTELSYS_NORMAL_PDF_H

#include "pastel/sys/random/normal_distribution.h"

#include <cmath>

namespace Pastel
{

	// Probability log-density of normal distribution.
	template <typename Real>
	Real logPdf(
		const Normal_Distribution<Real>& distribution,
		const arma::Col<Real>& x)
	{
		auto rotated = distribution.rotation() * (x - distribution.mean());

		return std::log(2 * constantPi<Real>()) * (-(Real)distribution.n() / 2) +
			std::log(distribution.detCovariance()) / 2 -
			trans(rotated) /
			distribution.scale() * 
			rotated / 2;
	}

	// Probability density of normal distribution.
	template <typename Real>
	Real pdf(
		const Normal_Distribution<Real>& distribution,
		const arma::Col<Real>& x)
	{
		return std::exp(logPdf(distribution, x));
	}

}

#endif
