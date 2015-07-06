// Description: Testing for random variables
// Documentation: unit_testing.txt

#include "test_pastelsys.h"
#include <pastel/sys/random.h>
#include <vector>

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			testRandomInteger();
		}

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

		void testRandomInteger()
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
				TEST_ENSURE(test(1));
				TEST_ENSURE(test(2));
				TEST_ENSURE(test(3));
				TEST_ENSURE(test(4));
				TEST_ENSURE(test(5));
				TEST_ENSURE(test(6));
				TEST_ENSURE(test(7));
				TEST_ENSURE(test(8));
				TEST_ENSURE(test(9));
				TEST_ENSURE(test(10));
				TEST_ENSURE(test(11));
				TEST_ENSURE(test(12));
				TEST_ENSURE(test(13));
				TEST_ENSURE(test(14));
				TEST_ENSURE(test(15));
				TEST_ENSURE(test(16));
			}
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("random", test);
	}

	CallFunction run(addTest);

}
