// Description: Testing for integer complexity
// DocumentationOf: integer_complexity.h

#include "test_pastelsys.h"

#include <pastel/sys/bit/integer_complexity.h>
#include <pastel/sys/pairwise_all_of.h>

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
			test();
		}

		void test()
		{
			auto f = [&](uint8 x, integer correct)
			{
				return integerComplexity(x) == correct;
			};

			TEST_ENSURE(f(0, 0));
			TEST_ENSURE(f(1, 1));
			TEST_ENSURE(f(2, 2));
			TEST_ENSURE(f(3, 1));
			TEST_ENSURE(f(4, 2));
			TEST_ENSURE(f(5, 3));
			TEST_ENSURE(f(6, 2));
			TEST_ENSURE(f(7, 1));
			TEST_ENSURE(f(8, 2));
			TEST_ENSURE(f(9, 3));
			TEST_ENSURE(f(10, 4));
			TEST_ENSURE(f(11, 3));
			TEST_ENSURE(f(12, 2));
			TEST_ENSURE(f(13, 3));
			TEST_ENSURE(f(14, 2));
			TEST_ENSURE(f(15, 1));
			TEST_ENSURE(f(16, 2));
			TEST_ENSURE(f(17, 3));
			TEST_ENSURE(f(18, 4));
			TEST_ENSURE(f(19, 3));
			TEST_ENSURE(f(20, 4));
			TEST_ENSURE(f(21, 5));
			TEST_ENSURE(f(22, 4));
			TEST_ENSURE(f(23, 3));
			TEST_ENSURE(f(24, 2));
			TEST_ENSURE(f(25, 3));
			TEST_ENSURE(f(26, 4));
			TEST_ENSURE(f(27, 3));
			TEST_ENSURE(f(28, 2));
			TEST_ENSURE(f(29, 3));
			TEST_ENSURE(f(30, 2));
			TEST_ENSURE(f(31, 1));
			TEST_ENSURE(f(255, 1));
			TEST_ENSURE(f(254, 2));
			TEST_ENSURE(f(128, 2));
			TEST_ENSURE(f(127, 1));

			uint8 numberSet[] = 
			{
				0, 1, 3, 2, 7, 6, 4, 5, 15, 14, 12, 13, 8, 9, 11, 10
			};

			TEST_ENSURE(pairwiseAllOf(range(numberSet), 
				[](uint8 left, uint8 right) 
				{
					return lessComplexInteger(left, right) &&
						!lessComplexInteger(right, left);
				}));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("integerComplexity", test);
	}

	CallFunction run(addTest);

}
