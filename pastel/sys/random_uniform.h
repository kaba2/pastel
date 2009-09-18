#ifndef PASTEL_RANDOM_UNIFORM_H
#define PASTEL_RANDOM_UNIFORM_H

#include "pastel/sys/mytypes.h"

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

	//! Returns a uniformly distributed random integer in [0, 0x7FFFFFFF].

	PASTELSYS integer randomInteger();

}

#include "pastel/sys/random_uniform.hpp"

#endif
