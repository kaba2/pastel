// Description: Testing for rounding
// DocumentationOf: rounding.h

#include "test/test_init.h"

#include <pastel/sys/math/rounding.h>
#include <pastel/sys/math/powers.h>

TEST_CASE("RoundUpTo (rounding)")
{
	REQUIRE(roundUpTo(-6, 3) == -6);
	REQUIRE(roundUpTo(-5, 3) == -3);
	REQUIRE(roundUpTo(-4, 3) == -3);
	REQUIRE(roundUpTo(-3, 3) == -3);
	REQUIRE(roundUpTo(-2, 3) == 0);
	REQUIRE(roundUpTo(-1, 3) == 0);
	REQUIRE(roundUpTo(+0, 3) == 0);
	REQUIRE(roundUpTo(+1, 3) == 3);
	REQUIRE(roundUpTo(+2, 3) == 3);
	REQUIRE(roundUpTo(+3, 3) == 3);
	REQUIRE(roundUpTo(+4, 3) == 6);
	REQUIRE(roundUpTo(+5, 3) == 6);
	REQUIRE(roundUpTo(+6, 3) == 6);

	for (integer i = 1; i < 10; ++i)
	{
		integer k = -4 * i;
		integer m = 0;
		for (integer j = -5 * i + 1; j <= 5 * i; ++j)
		{
			//std::cout << "roundUpTo(" << j << ", " << i 
			// << ") = " << roundUpTo(j, i) << std::endl;
			REQUIRE(roundUpTo(j, i) == k);

			++m;
			if (m == i)
			{
				m = 0;
				k += i;
			}
		}
	}
}

TEST_CASE("RoundUpToPowerOfTwo (rounding)")
{
	REQUIRE(roundUpToPowerOfTwo(-6, 2) == -4);
	REQUIRE(roundUpToPowerOfTwo(-5, 2) == -4);
	REQUIRE(roundUpToPowerOfTwo(-4, 2) == -4);
	REQUIRE(roundUpToPowerOfTwo(-3, 2) == +0);
	REQUIRE(roundUpToPowerOfTwo(-2, 2) == +0);
	REQUIRE(roundUpToPowerOfTwo(-1, 2) == +0);
	REQUIRE(roundUpToPowerOfTwo(+0, 2) == +0);
	REQUIRE(roundUpToPowerOfTwo(+1, 2) == +4);
	REQUIRE(roundUpToPowerOfTwo(+2, 2) == +4);
	REQUIRE(roundUpToPowerOfTwo(+3, 2) == +4);
	REQUIRE(roundUpToPowerOfTwo(+4, 2) == +4);
	REQUIRE(roundUpToPowerOfTwo(+5, 2) == +8);
	REQUIRE(roundUpToPowerOfTwo(+6, 2) == +8);

	for (integer n = 0; n < 5; ++n)
	{
		integer i = powerOfTwo<integer>(n);
		integer k = -4 * i;
		integer m = 0;
		for (integer j = -5 * i + 1; j <= 5 * i; ++j)
		{
			//std::cout << "roundUpTo(" << j << ", " << i 
			// << ") = " << roundUpTo(j, i) << std::endl;
			REQUIRE(roundUpToPowerOfTwo(j, n) == k);

			++m;
			if (m == i)
			{
				m = 0;
				k += i;
			}
		}
	}

	REQUIRE(roundUpToPowerOfTwo(0) == 1);
	REQUIRE(roundUpToPowerOfTwo(1) == 1);
	REQUIRE(roundUpToPowerOfTwo(2) == 2);
	REQUIRE(roundUpToPowerOfTwo(3) == 4);
	REQUIRE(roundUpToPowerOfTwo(5) == 8);
	REQUIRE(roundUpToPowerOfTwo(6) == 8);
	REQUIRE(roundUpToPowerOfTwo(8) == 8);
	REQUIRE(roundUpToPowerOfTwo(9) == 16);
	REQUIRE(roundUpToPowerOfTwo(9) == 16);
	REQUIRE(roundUpToPowerOfTwo(10) == 16);
	REQUIRE(roundUpToPowerOfTwo(11) == 16);
	REQUIRE(roundUpToPowerOfTwo(12) == 16);
	REQUIRE(roundUpToPowerOfTwo(13) == 16);
	REQUIRE(roundUpToPowerOfTwo(14) == 16);
	REQUIRE(roundUpToPowerOfTwo(15) == 16);
	REQUIRE(roundUpToPowerOfTwo(16) == 16);
	REQUIRE(roundUpToPowerOfTwo(0x0000000F) == 0x00000010);
	REQUIRE(roundUpToPowerOfTwo(0x000000FF) == 0x00000100);
	REQUIRE(roundUpToPowerOfTwo(0x00000FFF) == 0x00001000);
	REQUIRE(roundUpToPowerOfTwo(0x0000FFFF) == 0x00010000);
	REQUIRE(roundUpToPowerOfTwo(0x000FFFFF) == 0x00100000);
	REQUIRE(roundUpToPowerOfTwo(0x00FFFFFF) == 0x01000000);
	REQUIRE(roundUpToPowerOfTwo(0x0FFFFFFF) == 0x10000000);
}

TEST_CASE("RoundUpToEvenAndOdd (rounding)")
{
	REQUIRE(roundUpToEven(-4) == -4);
	REQUIRE(roundUpToEven(-3) == -2);
	REQUIRE(roundUpToEven(-2) == -2);
	REQUIRE(roundUpToEven(-1) == +0);
	REQUIRE(roundUpToEven(+0) == +0);
	REQUIRE(roundUpToEven(+1) == +2);
	REQUIRE(roundUpToEven(+2) == +2);
	REQUIRE(roundUpToEven(+3) == +4);
	REQUIRE(roundUpToEven(+4) == +4);

	REQUIRE(roundUpToOdd(-4) == -3);
	REQUIRE(roundUpToOdd(-3) == -3);
	REQUIRE(roundUpToOdd(-2) == -1);
	REQUIRE(roundUpToOdd(-1) == -1);
	REQUIRE(roundUpToOdd(+0) == +1);
	REQUIRE(roundUpToOdd(+1) == +1);
	REQUIRE(roundUpToOdd(+2) == +3);
	REQUIRE(roundUpToOdd(+3) == +3);
	REQUIRE(roundUpToOdd(+4) == +5);
}

TEST_CASE("DivideAndRoundUp (rounding)")
{
	REQUIRE(divideAndRoundUp(-7, 3) == -2);
	REQUIRE(divideAndRoundUp(-6, 3) == -2);
	REQUIRE(divideAndRoundUp(-5, 3) == -1);
	REQUIRE(divideAndRoundUp(-4, 3) == -1);
	REQUIRE(divideAndRoundUp(-3, 3) == -1);
	REQUIRE(divideAndRoundUp(-2, 3) == 0);
	REQUIRE(divideAndRoundUp(-1, 3) == 0);
	REQUIRE(divideAndRoundUp(0, 3) == 0);
	REQUIRE(divideAndRoundUp(1, 3) == 1);
	REQUIRE(divideAndRoundUp(2, 3) == 1);
	REQUIRE(divideAndRoundUp(3, 3) == 1);
	REQUIRE(divideAndRoundUp(4, 3) == 2);
	REQUIRE(divideAndRoundUp(5, 3) == 2);
	REQUIRE(divideAndRoundUp(6, 3) == 2);
	REQUIRE(divideAndRoundUp(7, 3) == 3);

	REQUIRE(divideAndRoundUp(-7, -3) == 3);
	REQUIRE(divideAndRoundUp(-6, -3) == 2);
	REQUIRE(divideAndRoundUp(-5, -3) == 2);
	REQUIRE(divideAndRoundUp(-4, -3) == 2);
	REQUIRE(divideAndRoundUp(-3, -3) == 1);
	REQUIRE(divideAndRoundUp(-2, -3) == 1);
	REQUIRE(divideAndRoundUp(-1, -3) == 1);
	REQUIRE(divideAndRoundUp(0, -3) == 0);
	REQUIRE(divideAndRoundUp(1, -3) == 0);
	REQUIRE(divideAndRoundUp(2, -3) == 0);
	REQUIRE(divideAndRoundUp(3, -3) == -1);
	REQUIRE(divideAndRoundUp(4, -3) == -1);
	REQUIRE(divideAndRoundUp(5, -3) == -1);
	REQUIRE(divideAndRoundUp(6, -3) == -2);
	REQUIRE(divideAndRoundUp(7, -3) == -2);
}
