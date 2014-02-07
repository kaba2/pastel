// Description: Testing for mod
// DocumentationOf: mod.h

#include "test_pastelsys.h"

#include <pastel/sys/mod.h>
#include <pastel/sys/powers.h>

using namespace Pastel;
using namespace std;

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
			testSignedMod();
			testUnsignedMod();
		}

		void testSignedMod()
		{
			for (integer i = 1; i < 10;++i)
			{
				integer k = 0;
				for (integer j = -5 * i;j <= 5 * i;++j)
				{
					TEST_ENSURE_OP(mod(j, i), ==, k);
					++k;
					if (k == i)
					{
						k = 0;
					}
				}
			}

			for (integer n = 0; n < 5;++n)
			{
				integer i = (1 << n);
				integer k = 0;
				for (integer j = -5 * i;j <= 5 * i;++j)
				{
					TEST_ENSURE_OP(modPowerOfTwo(j, n), ==, k);
					++k;
					if (k == i)
					{
						k = 0;
					}
				}
			}
		}

		void testUnsignedMod()
		{
			for (integer n = 0; n < 5;++n)
			{
				uinteger i = powerOfTwo<uinteger>(n);
				uinteger k = 0;
				for (uinteger j = 0;j <= 5 * i;++j)
				{
					TEST_ENSURE_OP(modPowerOfTwo(j, n), ==, k);
					++k;
					if (k == i)
					{
						k = 0;
					}
				}
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
		testRunner().add("mod", test);
	}

	CallFunction run(addTest);

}