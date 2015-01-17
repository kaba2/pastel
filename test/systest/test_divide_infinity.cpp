// Description: Testing for dividing infinity.
// DocumentationOf: divide_infinity.h

#include "test_pastelsys.h"
#include <pastel/sys/math/divide_infinity.h>

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
			{
				auto test = [&](uint8 n, uint8 correct)
				{
					return divideInfinity(n) == correct;
				};

				TEST_ENSURE(test(2, 128));
				TEST_ENSURE(test(3, 85));
				TEST_ENSURE(test(4, 64));
				TEST_ENSURE(test(5, 51));
				TEST_ENSURE(test(6, 42));
				TEST_ENSURE(test(7, 36));
				TEST_ENSURE(test(8, 32));
				TEST_ENSURE(test(9, 28));
				TEST_ENSURE(test(10, 25));
				TEST_ENSURE(test(11, 23));
				TEST_ENSURE(test(12, 21));
				TEST_ENSURE(test(13, 19));
				TEST_ENSURE(test(14, 18));
				TEST_ENSURE(test(15, 17));
				TEST_ENSURE(test(16, 16));
				TEST_ENSURE(test(17, 15));
				TEST_ENSURE(test(18, 14));
				TEST_ENSURE(test(19, 13));
				TEST_ENSURE(test(20, 12));
				TEST_ENSURE(test(21, 12));
				TEST_ENSURE(test(22, 11));
				TEST_ENSURE(test(23, 11));
				TEST_ENSURE(test(24, 10));
				TEST_ENSURE(test(25, 10));
				TEST_ENSURE(test(26, 9));
				TEST_ENSURE(test(27, 9));
				TEST_ENSURE(test(28, 9));
				TEST_ENSURE(test(29, 8));
				TEST_ENSURE(test(30, 8));
				TEST_ENSURE(test(31, 8));
				TEST_ENSURE(test(32, 8));

				TEST_ENSURE(test(64, 4));
				TEST_ENSURE(test(128, 2));
				TEST_ENSURE(test(255, 1));
			}

			{
				auto test = [&](uint16 n, uint16 correct)
				{
					return divideInfinity(n) == correct;
				};

				TEST_ENSURE(test(2, 32768));
				TEST_ENSURE(test(3, 21845));
				TEST_ENSURE(test(4, 16384));
				TEST_ENSURE(test(5, 13107));
				TEST_ENSURE(test(6, 10922));
				TEST_ENSURE(test(7, 9362));
				TEST_ENSURE(test(8, 8192));
				TEST_ENSURE(test(9, 7281));
				TEST_ENSURE(test(10, 6553));
				TEST_ENSURE(test(11, 5957));
				TEST_ENSURE(test(12, 5461));
				TEST_ENSURE(test(13, 5041));
				TEST_ENSURE(test(14, 4681));
				TEST_ENSURE(test(15, 4369));
				TEST_ENSURE(test(16, 4096));
				TEST_ENSURE(test(17, 3855));
				TEST_ENSURE(test(18, 3640));
				TEST_ENSURE(test(19, 3449));
				TEST_ENSURE(test(20, 3276));
				TEST_ENSURE(test(21, 3120));
				TEST_ENSURE(test(22, 2978));
				TEST_ENSURE(test(23, 2849));
				TEST_ENSURE(test(24, 2730));
				TEST_ENSURE(test(25, 2621));
				TEST_ENSURE(test(26, 2520));
				TEST_ENSURE(test(27, 2427));
				TEST_ENSURE(test(28, 2340));
				TEST_ENSURE(test(29, 2259));
				TEST_ENSURE(test(30, 2184));
				TEST_ENSURE(test(31, 2114));
				TEST_ENSURE(test(32, 2048));

				TEST_ENSURE(test(16384, 4));
				TEST_ENSURE(test(32768, 2));
				TEST_ENSURE(test(65535, 1));
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
		testRunner().add("divideInfinity", test);
	}

	CallFunction run(addTest);

}
