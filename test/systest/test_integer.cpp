// Description: Testing for Integer
// DocumentationOf: integer.h

#include "test_pastelsys.h"

#include "pastel/sys/integer.h"

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
			test();
		}

		void test()
		{
			{
				using F = Integer<1, uint8>;

				TEST_ENSURE(F(0) + F(0) == F(0));
				TEST_ENSURE(F(0) + F(1) == F(1));
				TEST_ENSURE(F(1) + F(0) == F(1));
				TEST_ENSURE(F(1) + F(1) == F(0));

				TEST_ENSURE(!F(0).test(0));
				TEST_ENSURE(F(1).test(0));

				TEST_ENSURE(F(0).set(0).test(0));
				TEST_ENSURE(F(0).set(0, true).test(0));
				TEST_ENSURE(!F(1).set(0, false).test(0));
				TEST_ENSURE(!F(1).reset(0).test(0));

				TEST_ENSURE_OP(F(0).count(), ==, 0);
				TEST_ENSURE_OP(F(1).count(), ==, 1);
			}

			{
				using F = Integer<2, uint8>;

				TEST_ENSURE(F(0) + F(0) == F(0));
				TEST_ENSURE(F(0) + F(1) == F(1));
				TEST_ENSURE(F(0) + F(2) == F(2));
				TEST_ENSURE(F(0) + F(3) == F(3));
				TEST_ENSURE(F(1) + F(0) == F(1));
				TEST_ENSURE(F(1) + F(1) == F(2));
				TEST_ENSURE(F(1) + F(2) == F(3));
				TEST_ENSURE(F(1) + F(3) == F(0));
				TEST_ENSURE(F(2) + F(0) == F(2));
				TEST_ENSURE(F(2) + F(1) == F(3));
				TEST_ENSURE(F(2) + F(2) == F(0));
				TEST_ENSURE(F(2) + F(3) == F(1));
				TEST_ENSURE(F(3) + F(0) == F(3));
				TEST_ENSURE(F(3) + F(1) == F(0));
				TEST_ENSURE(F(3) + F(2) == F(1));
				TEST_ENSURE(F(3) + F(3) == F(2));

				TEST_ENSURE(F(3).any());
				TEST_ENSURE(!F(0).any());

				TEST_ENSURE(!F(3).none());
				TEST_ENSURE(F(0).none());
			}

			{
				using F = Integer<32, uint8>;

				TEST_ENSURE_OP(F(0x12345678).count(), ==, 
					1 + 1 + 2 + 1 + 2 + 2 + 3 + 1);

				TEST_ENSURE(F(0x9ABCDEF0).to_string() ==
					"10011010101111001101111011110000");

				TEST_ENSURE_OP(F(0x9ABCDEF0).count(), ==, 
					2 + 2 + 3 + 2 + 3 + 3 + 4 + 0);
			}

			{
				using F = Integer<32, uint8>;

				uint32 a = 0x12345678;
				for (integer i = 0;i < 32;++i)
				{
					// Interesting fact:
					// if i ranges up to 33, then the following
					// tests fail for i == 32. This probably has 
					// to do with the shifts (a << i) and (a >> i) 
					// being undefined in this case.

					TEST_ENSURE(
						(F(a) << i) == F(a << i));

					TEST_ENSURE(
						(F(a) >> i) == F(a >> i));
				}

				// We define the out-of-range shift anyway.
				TEST_ENSURE((F(a) << 33) == F(0));
				TEST_ENSURE((F(a) >> 33) == F(0));
			}

			{
				using F = Integer<32, uint8>;
				F a(0x12345678);

				TEST_ENSURE(F(a, 0, 0) == F(0x00000000));
				TEST_ENSURE(F(a, 0, 1) == F(0x00000000));
				TEST_ENSURE(F(a, 0, 2) == F(0x00000000));
				TEST_ENSURE(F(a, 0, 3) == F(0x00000000));
				TEST_ENSURE(F(a, 0, 4) == F(0x00000008));
				TEST_ENSURE(F(a, 0, 5) == F(0x00000018));
				TEST_ENSURE(F(a, 0, 6) == F(0x00000038));
				TEST_ENSURE(F(a, 0, 7) == F(0x00000078));
				
				TEST_ENSURE(F(a, 0, 8) == F(0x00000078));
				TEST_ENSURE(F(a, 0, 9) == F(0x00000078));
				TEST_ENSURE(F(a, 0, 10) == F(0x00000278));
				TEST_ENSURE(F(a, 0, 11) == F(0x00000678));
				TEST_ENSURE(F(a, 0, 12) == F(0x00000678));
				TEST_ENSURE(F(a, 0, 13) == F(0x00001678));
				TEST_ENSURE(F(a, 0, 14) == F(0x00001678));
				TEST_ENSURE(F(a, 0, 15) == F(0x00005678));

				TEST_ENSURE(F(a, 0, 16) == F(0x00005678));
				TEST_ENSURE(F(a, 1, 16) == F(0x00005678));
				TEST_ENSURE(F(a, 2, 16) == F(0x00005678));
				TEST_ENSURE(F(a, 3, 16) == F(0x00005678));
				TEST_ENSURE(F(a, 4, 16) == F(0x00005670));
				TEST_ENSURE(F(a, 5, 16) == F(0x00005660));
				TEST_ENSURE(F(a, 6, 16) == F(0x00005640));
				TEST_ENSURE(F(a, 7, 16) == F(0x00005600));

				TEST_ENSURE(F(a, 8, 16) == F(0x00005600));
				TEST_ENSURE(F(a, 9, 16) == F(0x00005600));
				TEST_ENSURE(F(a, 10, 16) == F(0x00005400));
				TEST_ENSURE(F(a, 11, 16) == F(0x00005000));
				TEST_ENSURE(F(a, 12, 16) == F(0x00005000));
				TEST_ENSURE(F(a, 13, 16) == F(0x00004000));
				TEST_ENSURE(F(a, 14, 16) == F(0x00004000));
				TEST_ENSURE(F(a, 15, 16) == F(0x00000000));
				
				TEST_ENSURE(F(a, 16, 16) == F(0x00000000));
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
		testRunner().add("Integer", test);
	}

	CallFunction run(addTest);

}
