// Description: Uniformly-distributed random numbers

#ifndef PASTEL_RANDOM_UNIFORM_H
#define PASTEL_RANDOM_UNIFORM_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/mt19937.h"

namespace Pastel
{

	//! Returns a uniformly distributed random integer in [0, 0x7fffffff].
	inline integer randomInteger();

	//! Returns a uniformly distributed random integer in [min, max].
	inline integer randomInteger(integer min, integer max);

	//! Returns a random real in [0, 1] with 32-bit resolution.
	template <typename Real>
	Real random();

	//! Returns a random real in [0, 1[ with 32-bit resolution.
	template <typename Real>
	Real random0();

	//! Returns a random real in ]0, 1[ with 32-bit resolution.
	template <typename Real>
	Real random1();

	//! Returns a random real in [0, 1] with 53-bit resolution.
	template <typename Real>
	Real random53() ;

	//! Returns a uniformly distributed random real in [minValue, maxValue].
	/*!
	Time complexity: constant
	Exception safety: nothrow
	*/
	template <typename Real>
	Real random(
		const PASTEL_NO_DEDUCTION(Real)& minValue, 
		const PASTEL_NO_DEDUCTION(Real)& maxValue);

	//! Returns a random vector uniformly distributed in [0, 1]^N.
	template <typename Real, int N>
	Vector<Real, N> randomVector();

	//! Returns a random vector uniformly distributed in [0, 1]^N.
	/*!
	Preconditions:
	dimension >= 0
	*/
	template <typename Real, int N>
	Vector<Real, N> randomVector(integer dimension);

	//! Probability density function of the Uniform distribution.
	/*!
	This function assumes the probability distribution
	has support [0, 1].
	*/
	template <typename Real>
	Real uniformPdf(
		const PASTEL_NO_DEDUCTION(Real)& x);

	//! Probability density function of the Uniform distribution.
	/*!
	Preconditions:
	minValue < maxValue
	*/
	template <typename Real>
	Real uniformPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& minValue,
		const PASTEL_NO_DEDUCTION(Real)& maxValue);

}

#include "pastel/sys/random_uniform.hpp"

#endif
