// Description: Testing for LargeInteger
// DocumentationOf: largeinteger.h

#include "test_pastelsys.h"

#include "pastel/sys/largeinteger.h"

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
				using F = LargeInteger<1>;

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
				using F = LargeInteger<2>;

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
				using F = LargeInteger<32>;

				TEST_ENSURE_OP(F(0x12345678).count(), ==, 
					1 + 1 + 2 + 1 + 2 + 2 + 3 + 1);

				TEST_ENSURE(F(0x9ABCDEF0).to_string() ==
					"10011010101111001101111011110000");

				TEST_ENSURE_OP(F(0x9ABCDEF0).count(), ==, 
					2 + 2 + 3 + 2 + 3 + 3 + 4 + 0);
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
		testRunner().add("LargeInteger", test);
	}

	CallFunction run(addTest);

}
