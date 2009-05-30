#include "pastel/sys/random.h"

#include <boost/random.hpp>

namespace Pastel
{

	namespace Detail_Random
	{

		typedef boost::mt19937 EngineType;
		EngineType engine(42);

		PASTELSYS real32 randomReal32()
		{
			typedef boost::uniform_real<real32> DistributionType;
			typedef boost::variate_generator<EngineType&,
				DistributionType> GeneratorType;

			static DistributionType distribution(0, 1);
			static GeneratorType generator(engine, distribution);

			return generator();
		}

		PASTELSYS real64 randomReal64()
		{
			typedef boost::uniform_real<real64> DistributionType;
			typedef boost::variate_generator<EngineType&,
				DistributionType> GeneratorType;

			static DistributionType distribution(0, 1);
			static GeneratorType generator(engine, distribution);

			return generator();
		}

		PASTELSYS real32 randomGaussianReal32()
		{
			typedef boost::normal_distribution<real32> DistributionType;
			typedef boost::variate_generator<EngineType&,
				DistributionType> GeneratorType;

			static DistributionType distribution(0, 1);
			static GeneratorType generator(engine, distribution);

			return generator();
		}

		PASTELSYS real64 randomGaussianReal64()
		{
			typedef boost::normal_distribution<real64> DistributionType;
			typedef boost::variate_generator<EngineType&,
				DistributionType> GeneratorType;

			static DistributionType distribution(0, 1);
			static GeneratorType generator(engine, distribution);

			return generator();
		}

		PASTELSYS real32 randomExponentialReal32()
		{
			typedef boost::exponential_distribution<real32> DistributionType;
			typedef boost::variate_generator<EngineType&,
				DistributionType> GeneratorType;

			static DistributionType distribution(1);
			static GeneratorType generator(engine, distribution);

			return generator();
		}

		PASTELSYS real64 randomExponentialReal64()
		{
			typedef boost::exponential_distribution<real64> DistributionType;
			typedef boost::variate_generator<EngineType&,
				DistributionType> GeneratorType;

			static DistributionType distribution(1);
			static GeneratorType generator(engine, distribution);

			return generator();
		}

	}

	PASTELSYS integer randomInteger()
	{
		typedef boost::uniform_int<integer> DistributionType;
		typedef boost::variate_generator<Detail_Random::EngineType&,
			DistributionType> GeneratorType;

		static DistributionType distribution(0, 0x7FFFFFFF);
		static GeneratorType generator(Detail_Random::engine, distribution);

		return generator();
	}

	// RandomRealGenerator

	RandomRealGenerator::RandomRealGenerator()
		: engine_(42)
		, distribution_(0, 1)
		, generator_(engine_, distribution_)
	{
	}

	RandomRealGenerator::RandomRealGenerator(integer seed)
		: engine_((unsigned int)seed)
		, distribution_(0, 1)
		, generator_(engine_, distribution_)
	{
	}

	real RandomRealGenerator::operator ()()
	{
		return generator_();
	}

	// RandomIntegerGenerator

	RandomIntegerGenerator::RandomIntegerGenerator()
		: engine_(42)
		, distribution_(0, 0x7FFFFFFF)
		, generator_(engine_, distribution_)
	{
	}

	RandomIntegerGenerator::RandomIntegerGenerator(integer seed)
		: engine_((unsigned int)seed)
		, distribution_(0, 0x7FFFFFFF)
		, generator_(engine_, distribution_)
	{
	}

	integer RandomIntegerGenerator::operator ()()
	{
		return generator_();
	}

}
