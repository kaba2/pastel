/*!
\file
\brief Tools for random number generation.
*/

#ifndef PASTELSYS_RANDOM_H
#define PASTELSYS_RANDOM_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"

#include <boost/random.hpp>
#include <boost/utility/enable_if.hpp>

namespace Pastel
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

	//! Returns a uniformly distributed random real in [0, 1].
	/*!
	Time complexity: constant
	Exception safety: nothrow
	*/

	PASTELSYS real randomReal();

	//! Returns a uniformly distributed random real in [minValue, maxValue].
	/*!
	Time complexity: constant
	Exception safety: nothrow
	*/
	PASTELSYS real randomReal(
		real minValue, 
		real maxValue);

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

	PASTELSYS real randomNormalReal();

	//! Returns a uniformly distributed random integer in [0, 0x7FFFFFFF].

	PASTELSYS integer randomInteger();
	/*!
	Time complexity: constant
	Exception safety: nothrow
	*/

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
