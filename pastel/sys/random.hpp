#ifndef PASTELSYS_RANDOM_HPP
#define PASTELSYS_RANDOM_HPP

#include "pastel/sys/random.h"

namespace Pastel
{

	namespace Detail_Random
	{

		//! Returns a uniformly distributed random real32 in [0, 1].
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/

		PASTELSYS real32 randomReal32();

		//! Returns a uniformly distributed random real64 in [0, 1].
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/

		PASTELSYS real64 randomReal64();

		//! Returns a (0, 1)-normal distributed random real.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/

		PASTELSYS real randomNormalReal32();

		//! Returns a (0, 1)-normal distributed random real.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/

		PASTELSYS real randomNormalReal64();

	}

	template <typename Real>
	typename boost::enable_if<boost::is_same<Real, real32>, real32>::type 
		random()
	{
		return Detail_Random::randomReal32();
	}

	template <typename Real>
	typename boost::enable_if<boost::is_same<Real, real64>, real64>::type 
		random()
	{
		return Detail_Random::randomReal64();
	}

	template <typename Real>
	Real random(
		const PASTEL_NO_DEDUCTION(Real)& minValue, 
		const PASTEL_NO_DEDUCTION(Real)& maxValue)
	{
		return minValue + Pastel::random<Real>() * (maxValue - minValue);
	}

	template <typename Real>
	typename boost::enable_if<boost::is_same<Real, real32>, real32>::type 
		randomNormal()
	{
		return Detail_Random::randomNormalReal32();
	}

	template <typename Real>
	typename boost::enable_if<boost::is_same<Real, real64>, real64>::type 
		randomNormal()
	{
		return Detail_Random::randomNormalReal64();
	}

}

#endif
