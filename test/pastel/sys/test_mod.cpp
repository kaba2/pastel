// Description: Testing for mod
// DocumentationOf: mod.h

#include "test/test_init.h"

#include <pastel/sys/math/mod.h>
#include <pastel/sys/math/powers.h>

namespace
{

	class Test
	{
	public:
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
					REQUIRE(mod(j, i) == k);
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
					REQUIRE(modPowerOfTwo(j, n) == k);
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
					REQUIRE(modPowerOfTwo(j, n) == k);
					++k;
					if (k == i)
					{
						k = 0;
					}
				}
			}
		}
	};

	TEST_CASE("mod", "[mod]")
	{
	}

}
