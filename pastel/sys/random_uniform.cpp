#include "pastel/sys/random_uniform.h"

#include <boost/random.hpp>

namespace Pastel
{

	namespace Detail_RandomUniform
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

	}

	PASTELSYS integer randomInteger()
	{
		typedef boost::uniform_int<integer> DistributionType;
		typedef boost::variate_generator<Detail_RandomUniform::EngineType&,
			DistributionType> GeneratorType;

		static DistributionType distribution(0, 0x7FFFFFFF);
		static GeneratorType generator(Detail_RandomUniform::engine, distribution);

		return generator();
	}

}
