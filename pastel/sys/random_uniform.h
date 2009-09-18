// Description: Uniformly-distributed random numbers

#ifndef PASTEL_RANDOM_UNIFORM_H
#define PASTEL_RANDOM_UNIFORM_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

namespace Pastel
{

	//! Returns a uniformly distributed random real in [0, 1].
	/*!
	Time complexity: constant
	Exception safety: nothrow
	*/

	template <typename Real>
	typename boost::enable_if<boost::is_same<Real, real32>, real32>::type 
		random();

	//! Returns a uniformly distributed random real in [0, 1].
	/*!
	Time complexity: constant
	Exception safety: nothrow
	*/

	template <typename Real>
	typename boost::enable_if<boost::is_same<Real, real64>, real64>::type 
		random();

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

	template <int N, typename Real>
	Vector<Real, N> randomVector();

	//! Returns a random vector uniformly distributed in [0, 1]^N.
	/*!
	Preconditions:
	dimension >= 0
	*/

	template <int N, typename Real>
	Vector<Real, N> randomVector(integer dimension);

	//! Returns a uniformly distributed random integer in [0, 0x7FFFFFFF].
	PASTELSYS integer randomInteger();

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
