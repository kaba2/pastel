#include "pastel/sys/random_generator.h"

#include <boost/random.hpp>

namespace Pastel
{

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
