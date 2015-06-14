// Description: Gamma-distributed random numbers

#ifndef PASTELSYS_RANDOM_GAMMA_H
#define PASTELSYS_RANDOM_GAMMA_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Returns a gamma-distributed random real with scale 1.
	/*!
	Preconditions:
	shape > 0
	*/
	template <typename Real>
	Real randomGamma(
		const NoDeduction<Real>& shape);

	//! Returns a gamma-distributed random real.
	/*!
	Preconditions:
	shape > 0
	scale > 0
	*/
	template <typename Real>
	Real randomGamma(
		const NoDeduction<Real>& shape,
		const NoDeduction<Real>& scale);

	//! Converts variance to the scale parameter of the gamma distribution.
	/*!
	Preconditions:
	shape > 0
	variance >= 0
	*/
	template <typename Real>
	Real varianceToGammaScale(
		const NoDeduction<Real>& shape,
		const NoDeduction<Real>& variance);

	//! Returns a random vector gamma distributed in each component.
	
	template <typename Real, integer N>
	Vector<Real, N> randomGammaVector(
		const NoDeduction<Real>& shape);

	//! Returns a random vector gamma distributed in each component.
	
	template <typename Real, integer N>
	Vector<Real, N> randomGammaVector(
		integer dimension,
		const NoDeduction<Real>& shape);

	//! Probability density function for the gamma distribution with scale 1.
	/*!
	Preconditions:
	shape > 0

	This function assumes scale = 1.
	*/
	template <typename Real>
	Real gammaPdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& shape);

	//! Probability density function for the gamma distribution.
	/*!
	Preconditions:
	shape > 0
	scale > 0
	*/
	template <typename Real>
	Real gammaPdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& shape,
		const NoDeduction<Real>& scale);

}

#include "pastel/sys/random/random_gamma.hpp"

#endif
