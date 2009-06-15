/*!
\file
\brief Tools for random number generation.
*/

#ifndef PASTEL_RANDOM_H
#define PASTEL_RANDOM_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"
#include "pastel/sys/vector.h"

#include <boost/random.hpp>
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

	//! Returns a (0, 1)-normal distributed random real.
	/*!
	Time complexity: constant
	Exception safety: nothrow
	*/

	template <typename Real>
	Real randomGaussian();

	template <typename Real>
	Real randomGaussian(
		const PASTEL_NO_DEDUCTION(Real)& mean,
		const PASTEL_NO_DEDUCTION(Real)& deviation);

	//! Returns an exponentially distributed random real.
	/*!
	Time complexity: constant
	Exception safety: nothrow
	*/

	template <typename Real>
	Real randomExponential();

	template <typename Real>
	Real randomExponential(
		const PASTEL_NO_DEDUCTION(Real)& mean);

	//! Returns a gamma distributed random real.

	template <typename Real>
	Real randomGamma(
		const PASTEL_NO_DEDUCTION(Real)& alpha);

	template <typename Real>
	Real randomGamma(
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale);

	template <typename Real>
	Real varianceToGammaScale(
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& variance);

	//! Returns a generalized normal distributed random real.
	/*!
	shape = 1: Laplace distribution.
	shape = 2: Standard normal distribution.
	shape -> oo: Uniform distribution in [-scale, scale] in limit.
	*/

	template <typename Real>
	Real randomGeneralizedGaussian(
		const PASTEL_NO_DEDUCTION(Real)& shape, 
		const PASTEL_NO_DEDUCTION(Real)& scale);

	template <typename Real>
	Real randomGeneralizedGaussian(
		const PASTEL_NO_DEDUCTION(Real)& shape);

	template <typename Real>
	Real varianceToGeneralizedGaussianScale(
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& variance);

	//! Returns a uniformly distributed random integer in [0, 0x7FFFFFFF].

	PASTELSYS integer randomInteger();

	//! A class for generating uniformly distributed random reals.

	/*!
	The distribution is in the range [0, 1].
	*/

	class PASTELSYS RandomRealGenerator
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		//! Constructs a generator with the seed 0.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		RandomRealGenerator();

		//! Constructs a generator with the given seed.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		explicit RandomRealGenerator(integer seed);

		//! Returns a number in [0, 1].
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		real operator()();

	private:
		typedef boost::mt19937 EngineType;
		typedef boost::uniform_real<> DistributionType;
		typedef boost::variate_generator<EngineType&,
			DistributionType> GeneratorType;

		EngineType engine_;
		DistributionType distribution_;
		GeneratorType generator_;
	};

	//! A class for generating uniformly distributed random integers.

	class PASTELSYS RandomIntegerGenerator
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		//! Constructs a generator with seed 0.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		RandomIntegerGenerator();

		//! Constructs a generator with the given seed.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		explicit RandomIntegerGenerator(integer seed);

		//! Returns a random integer.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer operator()();

	private:
		typedef boost::mt19937 EngineType;
		typedef boost::uniform_int<> DistributionType;
		typedef boost::variate_generator<EngineType&,
			DistributionType> GeneratorType;

		EngineType engine_;
		DistributionType distribution_;
		GeneratorType generator_;
	};

}

#include "pastel/sys/random.hpp"

#endif
