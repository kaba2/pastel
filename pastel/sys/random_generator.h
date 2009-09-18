#ifndef PASTEL_RANDOM_GENERATOR_H
#define PASTEL_RANDOM_GENERATOR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"

#include <boost/random.hpp>

namespace Pastel
{

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

#endif
