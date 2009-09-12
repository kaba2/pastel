// Description: Probability density functions

#ifndef PASTEL_RANDOM_PDF_H
#define PASTEL_RANDOM_PDF_H

#include "pastel/sys/mytypes.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	Real gaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x);

	template <typename Real>
	Real gaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& mean,
		const PASTEL_NO_DEDUCTION(Real)& deviation);

	template <typename Real>
	Real generalizedGaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale);

	template <typename Real>
	Real generalizedGaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape);

	template <typename Real>
	Real exponentialPdf(
		const PASTEL_NO_DEDUCTION(Real)& x);

	template <typename Real>
	Real exponentialPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& mean);

	template <typename Real>
	Real gammaPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape);

	template <typename Real>
	Real gammaPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale);

}

#include "pastel/sys/random_pdf.hpp"

#endif
