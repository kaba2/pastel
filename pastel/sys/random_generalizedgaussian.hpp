#ifndef	PASTEL_RANDOM_GENERALIZEDGAUSSIAN_HPP
#define PASTEL_RANDOM_GENERALIZEDGAUSSIAN_HPP

#include "pastel/sys/random_generalizedgaussian.h"
#include "pastel/sys/random_uniform.h"
#include "pastel/sys/random_gamma.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	Real randomGeneralizedGaussian(
		const PASTEL_NO_DEDUCTION(Real)& shape)
	{
		// See "Computer generation of the exponential power
		// distributions", Johnson, M. E., 
		// Journal of Statistical Computation and Simulation, 9,
		// pp. 239--240, 1979

		const Real invShape = inverse(shape);
		const Real x = std::pow(
			randomGamma<Real>(invShape), invShape);
		
		if (random<Real>() < 0.5)
		{
			return -x;
		}

		return x;
	}

	template <typename Real>
	Real randomGeneralizedGaussian(
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		return Pastel::randomGeneralizedGaussian<Real>(shape) * scale;
	}

	template <typename Real>
	Real varianceToGeneralizedGaussianScale(
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& variance)
	{
		PENSURE_OP(shape, >, 0);
		PENSURE_OP(variance, >=, 0);

		const Real invShape = inverse(shape);
		return std::sqrt(variance * gamma<Real>(invShape) / 
			gamma<Real>(invShape * 3));
	}

	template <typename Real>
	Real generalizedGaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		PENSURE_OP(shape, >, 0);
		PENSURE_OP(scale, >=, 0);

		return (shape / (2 * scale * gamma<Real>(inverse(shape)))) *
			std::exp(-std::pow((std::abs(x / scale)), shape));
	}

	template <typename Real>
	Real generalizedGaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape)
	{
		PENSURE_OP(shape, >, 0);

		return (shape / (2 * gamma<Real>(inverse(shape)))) *
			std::exp(-std::pow((std::abs(x)), shape));
	}

}

#endif
