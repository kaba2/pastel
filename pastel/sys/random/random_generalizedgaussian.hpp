#ifndef	PASTELSYS_RANDOM_GENERALIZEDGAUSSIAN_HPP
#define PASTELSYS_RANDOM_GENERALIZEDGAUSSIAN_HPP

#include "pastel/sys/random/random_generalizedgaussian.h"
#include "pastel/sys/random/random_uniform.h"
#include "pastel/sys/random/random_gamma.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	Real randomGeneralizedGaussian(
		const NoDeduction<Real>& shape)
	{
		PENSURE_OP(shape, >, 0);

		// See "Computer generation of the exponential power
		// distributions", Johnson, M. E., 
		// Journal of Statistical Computation and Simulation, 9,
		// pp. 239--240, 1979

		Real invShape = inverse(shape);
		Real x = std::pow(
			randomGamma<Real>(invShape), invShape);
		
		if (random<Real>() < 0.5)
		{
			return -x;
		}

		return x;
	}

	template <typename Real>
	Real randomGeneralizedGaussian(
		const NoDeduction<Real>& shape,
		const NoDeduction<Real>& scale)
	{
		return Pastel::randomGeneralizedGaussian<Real>(shape) * scale;
	}

	template <typename Real>
	Real varianceToGeneralizedGaussianScale(
		const NoDeduction<Real>& shape,
		const NoDeduction<Real>& variance)
	{
		PENSURE_OP(shape, >, 0);
		PENSURE_OP(variance, >=, 0);

		Real invShape = inverse(shape);

		return std::sqrt(variance * gamma<Real>(invShape) / 
			gamma<Real>(invShape * 3));
	}

	template <typename Real, int N>
	Vector<Real, N> randomGeneralizedGaussianVector(
		const NoDeduction<Real>& shape, 
		const NoDeduction<Real>& scale)
	{
		PASTEL_STATIC_ASSERT(N != Dynamic);

		return Pastel::randomGeneralizedGaussianVector<Real, N>(
			N, shape, scale);
	}

	template <typename Real, int N>
	Vector<Real, N> randomGeneralizedGaussianVector(
		integer dimension,
		const NoDeduction<Real>& shape, 
		const NoDeduction<Real>& scale)
	{
		PENSURE_OP(dimension, >=, 0);

		Vector<Real, N> result(ofDimension(dimension));
		for (integer i = 0;i < dimension;++i)
		{
			result[i] = randomGeneralizedGaussian<Real>(shape, scale);
		}
		
		return result;
	}

	template <typename Real>
	Real generalizedGaussianPdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& shape,
		const NoDeduction<Real>& scale)
	{
		PENSURE_OP(shape, >, 0);
		PENSURE_OP(scale, >=, 0);

		return (shape / (2 * scale * gamma<Real>(inverse(shape)))) *
			std::exp(-std::pow((std::abs(x / scale)), shape));
	}

	template <typename Real>
	Real generalizedGaussianPdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& shape)
	{
		PENSURE_OP(shape, >, 0);

		return (shape / (2 * gamma<Real>(inverse(shape)))) *
			std::exp(-std::pow((std::abs(x)), shape));
	}

}

#endif
