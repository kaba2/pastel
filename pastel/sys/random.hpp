#ifndef PASTELSYS_RANDOM_HPP
#define PASTELSYS_RANDOM_HPP

#include "pastel/sys/random.h"

namespace Pastel
{

	template <typename Real>
	typename boost::enable_if<boost::is_same<Real, real32>, real32>::type 
		random()
	{
		return randomReal32();
	}

	template <typename Real>
	typename boost::enable_if<boost::is_same<Real, real64>, real64>::type 
		random()
	{
		return randomReal64();
	}

	template <typename Real>
	Real randomReal(
		const PASTEL_NO_DEDUCTION(Real)& minValue, 
		const PASTEL_NO_DEDUCTION(Real)& maxValue)
	{
		return minValue + Pastel::random<Real>() * (maxValue - minValue);
	}

}

#endif
