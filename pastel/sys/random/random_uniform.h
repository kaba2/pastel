// Description: Uniformly-distributed random numbers

#ifndef PASTELSYS_RANDOM_UNIFORM_H
#define PASTELSYS_RANDOM_UNIFORM_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Returns a random dreal in [0, 1] with 32-bit resolution.
	template <typename Real>
	Real random();

	//! Returns a random dreal in ]0, 1[ with 32-bit resolution.
	template <typename Real>
	Real randomOpen();

	//! Returns a random dreal in ]0, 1] with 32-bit resolution.
	template <typename Real>
	Real randomOpen0();

	//! Returns a random dreal in [0, 1[ with 32-bit resolution.
	template <typename Real>
	Real randomOpen1();

	//! Returns a random dreal in [0, 1] with 53-bit resolution.
	template <typename Real>
	Real random53();

	//! Returns a uniformly distributed random dreal in [minValue, maxValue].
	/*!
	Time complexity: constant
	Exception safety: nothrow
	*/
	template <typename Real>
	Real random(
		const NoDeduction<Real>& minValue, 
		const NoDeduction<Real>& maxValue);

	//! Returns a random vector uniformly distributed in [0, 1]^N.
	template <typename Real, integer N>
	Vector<Real, N> randomVector();

	//! Returns a random vector uniformly distributed in [0, 1]^N.
	/*!
	Preconditions:
	dimension >= 0
	*/
	template <typename Real, integer N>
	Vector<Real, N> randomVector(integer dimension);

	//! Probability density function of the Uniform distribution.
	/*!
	This function assumes the probability distribution
	has support [0, 1].
	*/
	template <typename Real>
	Real uniformPdf(
		const NoDeduction<Real>& x);

	//! Probability density function of the Uniform distribution.
	/*!
	Preconditions:
	minValue < maxValue
	*/
	template <typename Real>
	Real uniformPdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& minValue,
		const NoDeduction<Real>& maxValue);

}

#include "pastel/sys/random/random_uniform.hpp"

#endif
