// Description: Gamma-distributed random numbers

#ifndef PASTEL_RANDOM_GAMMA_H
#define PASTEL_RANDOM_GAMMA_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns a gamma-distributed random real.
	/*!
	Preconditions:
	shape > 0
	*/
	template <typename Real>
	Real randomGamma(
		const PASTEL_NO_DEDUCTION(Real)& shape);

	//! Returns a gamma-distributed random real.
	/*!
	Preconditions:
	shape > 0
	scale >= 0

	This is a convenience function that calls
	randomGamma<Real>(shape) * scale
	*/
	template <typename Real>
	Real randomGamma(
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale);

	//! Converts variance to the shape parameter of the Gamma distribution.
	/*!
	Preconditions:
	shape > 0
	variance >= 0
	*/
	template <typename Real>
	Real varianceToGammaScale(
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& variance);

	//! Probability density function for the Gamma distribution.
	/*!
	Preconditions:
	shape > 0

	This function assumes scale = 1.
	*/
	template <typename Real>
	Real gammaPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape);

	//! Probability density function for the Gamma distribution.
	/*!
	Preconditions:
	shape > 0
	scale >= 0
	*/
	template <typename Real>
	Real gammaPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale);

}

#include "pastel/sys/random_gamma.hpp"

#endif
