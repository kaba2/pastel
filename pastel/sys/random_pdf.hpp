#ifndef PASTEL_RANDOM_PDF_HPP
#define PASTEL_RANDOM_PDF_HPP

#include "pastel/sys/random_pdf.h"

namespace Pastel
{

	template <typename Real>
	Real gaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x)
	{
		return inverse(std::sqrt(2 * constantPi<Real>())) * 
			std::exp(-square(x) / 2);
	}

	template <typename Real>
	Real gaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& mean,
		const PASTEL_NO_DEDUCTION(Real)& deviation)
	{
		PENSURE(deviation >= 0);

		return inverse(deviation * std::sqrt(2 * constantPi<Real>())) * 
			std::exp(-square(x / deviation) / 2);
	}

	template <typename Real>
	Real generalizedGaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		return (shape / (2 * scale * gamma<Real>(inverse(shape)))) *
			std::exp(-std::pow((std::abs(x / scale)), shape));
	}

	template <typename Real>
	Real generalizedGaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape)
	{
		return (shape / (2 * gamma<Real>(inverse(shape)))) *
			std::exp(-std::pow((std::abs(x)), shape));
	}

	template <typename Real>
	Real exponentialPdf(
		const PASTEL_NO_DEDUCTION(Real)& x)
	{
		if (x < 0)
		{
			return 0;
		}

		return std::exp(-x);
	}

	template <typename Real>
	Real exponentialPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& mean)
	{
		PENSURE(mean > 0);

		if (x < 0)
		{
			return 0;
		}

		return mean * std::exp(-mean * x);
	}

	template <typename Real>
	Real gammaPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape)
	{
		PENSURE(shape > 0);

		return std::pow(x, shape - 1) * 
			std::exp(-x) / gamma<Real>(shape);
	}

	template <typename Real>
	Real gammaPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		PENSURE(shape > 0);
		PENSURE(scale > 0);

		return std::pow(x, shape - 1) * 
			std::exp(-x / scale) / 
			(gamma<Real>(shape) * std::pow(scale, shape));
	}

}

#endif
