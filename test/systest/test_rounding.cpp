// Description: Testing for rounding
// DocumentationOf: rounding.h

#include "test_pastelsys.h"

#include <pastel/sys/rounding.h>
#include <pastel/sys/powers.h>

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
			testRoundUpTo();
			testRoundUpToPowerOfTwo();
			testRoundUpToEvenAndOdd();
			testDivideAndRoundUp();
		}

		void testRoundUpTo()
		{
			TEST_ENSURE_OP(roundUpTo(-6, 3), == , -6);
			TEST_ENSURE_OP(roundUpTo(-5, 3), == , -3);
			TEST_ENSURE_OP(roundUpTo(-4, 3), == , -3);
			TEST_ENSURE_OP(roundUpTo(-3, 3), == , -3);
			TEST_ENSURE_OP(roundUpTo(-2, 3), == , 0);
			TEST_ENSURE_OP(roundUpTo(-1, 3), == , 0);
			TEST_ENSURE_OP(roundUpTo(+0, 3), == , 0);
			TEST_ENSURE_OP(roundUpTo(+1, 3), == , 3);
			TEST_ENSURE_OP(roundUpTo(+2, 3), == , 3);
			TEST_ENSURE_OP(roundUpTo(+3, 3), == , 3);
			TEST_ENSURE_OP(roundUpTo(+4, 3), == , 6);
			TEST_ENSURE_OP(roundUpTo(+5, 3), == , 6);
			TEST_ENSURE_OP(roundUpTo(+6, 3), == , 6);

			for (integer i = 1; i < 10; ++i)
			{
				integer k = -4 * i;
				integer m = 0;
				for (integer j = -5 * i + 1; j <= 5 * i; ++j)
				{
					//std::cout << "roundUpTo(" << j << ", " << i 
					// << ") = " << roundUpTo(j, i) << std::endl;
					TEST_ENSURE_OP(roundUpTo(j, i), == , k);

					++m;
					if (m == i)
					{
						m = 0;
						k += i;
					}
				}
			}
		}

		void testRoundUpToPowerOfTwo()
		{
			TEST_ENSURE_OP(roundUpToPowerOfTwo(-6, 2), == , -4);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(-5, 2), == , -4);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(-4, 2), == , -4);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(-3, 2), == , +0);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(-2, 2), == , +0);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(-1, 2), == , +0);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(+0, 2), == , +0);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(+1, 2), == , +4);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(+2, 2), == , +4);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(+3, 2), == , +4);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(+4, 2), == , +4);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(+5, 2), == , +8);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(+6, 2), == , +8);

			for (integer n = 0; n < 5; ++n)
			{
				integer i = powerOfTwo<integer>(n);
				integer k = -4 * i;
				integer m = 0;
				for (integer j = -5 * i + 1; j <= 5 * i; ++j)
				{
					//std::cout << "roundUpTo(" << j << ", " << i 
					// << ") = " << roundUpTo(j, i) << std::endl;
					TEST_ENSURE_OP(roundUpToPowerOfTwo(j, n), == , k);

					++m;
					if (m == i)
					{
						m = 0;
						k += i;
					}
				}
			}

			TEST_ENSURE_OP(roundUpToPowerOfTwo(0), == , 1);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(1), == , 1);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(2), == , 2);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(3), == , 4);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(5), == , 8);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(6), == , 8);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(8), == , 8);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(9), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(9), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(10), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(11), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(12), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(13), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(14), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(15), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(16), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(0x0000000F), == , 0x00000010);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(0x000000FF), == , 0x00000100);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(0x00000FFF), == , 0x00001000);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(0x0000FFFF), == , 0x00010000);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(0x000FFFFF), == , 0x00100000);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(0x00FFFFFF), == , 0x01000000);
			TEST_ENSURE_OP(roundUpToPowerOfTwo(0x0FFFFFFF), == , 0x10000000);
		}

		void testRoundUpToEvenAndOdd()
		{
			TEST_ENSURE_OP(roundUpToEven(-4), == , -4);
			TEST_ENSURE_OP(roundUpToEven(-3), == , -2);
			TEST_ENSURE_OP(roundUpToEven(-2), == , -2);
			TEST_ENSURE_OP(roundUpToEven(-1), == , +0);
			TEST_ENSURE_OP(roundUpToEven(+0), == , +0);
			TEST_ENSURE_OP(roundUpToEven(+1), == , +2);
			TEST_ENSURE_OP(roundUpToEven(+2), == , +2);
			TEST_ENSURE_OP(roundUpToEven(+3), == , +4);
			TEST_ENSURE_OP(roundUpToEven(+4), == , +4);

			TEST_ENSURE_OP(roundUpToOdd(-4), == , -3);
			TEST_ENSURE_OP(roundUpToOdd(-3), == , -3);
			TEST_ENSURE_OP(roundUpToOdd(-2), == , -1);
			TEST_ENSURE_OP(roundUpToOdd(-1), == , -1);
			TEST_ENSURE_OP(roundUpToOdd(+0), == , +1);
			TEST_ENSURE_OP(roundUpToOdd(+1), == , +1);
			TEST_ENSURE_OP(roundUpToOdd(+2), == , +3);
			TEST_ENSURE_OP(roundUpToOdd(+3), == , +3);
			TEST_ENSURE_OP(roundUpToOdd(+4), == , +5);
		}

		void testDivideAndRoundUp()
		{
			TEST_ENSURE_OP(divideAndRoundUp(-7, 3), == , -2);
			TEST_ENSURE_OP(divideAndRoundUp(-6, 3), == , -2);
			TEST_ENSURE_OP(divideAndRoundUp(-5, 3), == , -1);
			TEST_ENSURE_OP(divideAndRoundUp(-4, 3), == , -1);
			TEST_ENSURE_OP(divideAndRoundUp(-3, 3), == , -1);
			TEST_ENSURE_OP(divideAndRoundUp(-2, 3), == , 0);
			TEST_ENSURE_OP(divideAndRoundUp(-1, 3), == , 0);
			TEST_ENSURE_OP(divideAndRoundUp(0, 3), == , 0);
			TEST_ENSURE_OP(divideAndRoundUp(1, 3), == , 1);
			TEST_ENSURE_OP(divideAndRoundUp(2, 3), == , 1);
			TEST_ENSURE_OP(divideAndRoundUp(3, 3), == , 1);
			TEST_ENSURE_OP(divideAndRoundUp(4, 3), == , 2);
			TEST_ENSURE_OP(divideAndRoundUp(5, 3), == , 2);
			TEST_ENSURE_OP(divideAndRoundUp(6, 3), == , 2);
			TEST_ENSURE_OP(divideAndRoundUp(7, 3), == , 3);

			TEST_ENSURE_OP(divideAndRoundUp(-7, -3), == , 3);
			TEST_ENSURE_OP(divideAndRoundUp(-6, -3), == , 2);
			TEST_ENSURE_OP(divideAndRoundUp(-5, -3), == , 2);
			TEST_ENSURE_OP(divideAndRoundUp(-4, -3), == , 2);
			TEST_ENSURE_OP(divideAndRoundUp(-3, -3), == , 1);
			TEST_ENSURE_OP(divideAndRoundUp(-2, -3), == , 1);
			TEST_ENSURE_OP(divideAndRoundUp(-1, -3), == , 1);
			TEST_ENSURE_OP(divideAndRoundUp(0, -3), == , 0);
			TEST_ENSURE_OP(divideAndRoundUp(1, -3), == , 0);
			TEST_ENSURE_OP(divideAndRoundUp(2, -3), == , 0);
			TEST_ENSURE_OP(divideAndRoundUp(3, -3), == , -1);
			TEST_ENSURE_OP(divideAndRoundUp(4, -3), == , -1);
			TEST_ENSURE_OP(divideAndRoundUp(5, -3), == , -1);
			TEST_ENSURE_OP(divideAndRoundUp(6, -3), == , -2);
			TEST_ENSURE_OP(divideAndRoundUp(7, -3), == , -2);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTests()
	{
		testRunner().add("rounding", test);
	}

	CallFunction run(addTests);

}
