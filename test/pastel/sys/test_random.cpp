// Description: Testing for random variables
// Documentation: unit_testing.txt

#include "test/test_init.h"
#include <pastel/sys/random.h>
#include <vector>

namespace
{

	std::vector<real> sample(integer size)
	{
		std::vector<real> pmf(size, 0);

		integer n = 100 * size;
		for (integer i = 0;i < n;++i)
		{
			++pmf[randomInteger(size)];
		}
		for (integer i = 0;i < size;++i)
		{
			pmf[i] /= n;
		}

		return pmf;
	}

}

TEST_CASE("RandomInteger (random)")
{
	auto test = [&](integer size)
	{
		std::vector<real> pmf = sample(size);

		for (integer i = 0;i < pmf.size();++i)
		{
			real correct = (real)1 / pmf.size();
			real error = absoluteError<real>(pmf[i], correct);
			if (error > 0.1)
			{
				/*
				std::cout << "error " << error << std::endl;
				for (real p : pmf)
				{
					std::cout << p << " ";
				}
				std::cout << std::endl;
				*/
				return false;
			}
		}
		return true;
	};

	{
		REQUIRE(test(1));
		REQUIRE(test(2));
		REQUIRE(test(3));
		REQUIRE(test(4));
		REQUIRE(test(5));
		REQUIRE(test(6));
		REQUIRE(test(7));
		REQUIRE(test(8));
		REQUIRE(test(9));
		REQUIRE(test(10));
		REQUIRE(test(11));
		REQUIRE(test(12));
		REQUIRE(test(13));
		REQUIRE(test(14));
		REQUIRE(test(15));
		REQUIRE(test(16));
	}
}

