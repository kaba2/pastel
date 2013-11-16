// Description: Testing for divideAndRoundUp()
// DocumentationOf: rounding.h

#include "test_pastelsys.h"

#include "pastel/sys/rounding.h"

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
			TEST_ENSURE_OP(divideAndRoundUp(-7, 3), ==, -2);
			TEST_ENSURE_OP(divideAndRoundUp(-6, 3), ==, -2);
			TEST_ENSURE_OP(divideAndRoundUp(-5, 3), ==, -1);
			TEST_ENSURE_OP(divideAndRoundUp(-4, 3), ==, -1);
			TEST_ENSURE_OP(divideAndRoundUp(-3, 3), ==, -1);
			TEST_ENSURE_OP(divideAndRoundUp(-2, 3), ==, 0);
			TEST_ENSURE_OP(divideAndRoundUp(-1, 3), ==, 0);
			TEST_ENSURE_OP(divideAndRoundUp(0, 3), ==, 0);
			TEST_ENSURE_OP(divideAndRoundUp(1, 3), ==, 1);
			TEST_ENSURE_OP(divideAndRoundUp(2, 3), ==, 1);
			TEST_ENSURE_OP(divideAndRoundUp(3, 3), ==, 1);
			TEST_ENSURE_OP(divideAndRoundUp(4, 3), ==, 2);
			TEST_ENSURE_OP(divideAndRoundUp(5, 3), ==, 2);
			TEST_ENSURE_OP(divideAndRoundUp(6, 3), ==, 2);
			TEST_ENSURE_OP(divideAndRoundUp(7, 3), ==, 3);

			TEST_ENSURE_OP(divideAndRoundUp(-7, -3), ==, 3);
			TEST_ENSURE_OP(divideAndRoundUp(-6, -3), ==, 2);
			TEST_ENSURE_OP(divideAndRoundUp(-5, -3), ==, 2);
			TEST_ENSURE_OP(divideAndRoundUp(-4, -3), ==, 2);
			TEST_ENSURE_OP(divideAndRoundUp(-3, -3), ==, 1);
			TEST_ENSURE_OP(divideAndRoundUp(-2, -3), ==, 1);
			TEST_ENSURE_OP(divideAndRoundUp(-1, -3), ==, 1);
			TEST_ENSURE_OP(divideAndRoundUp(0, -3), ==, 0);
			TEST_ENSURE_OP(divideAndRoundUp(1, -3), ==, 0);
			TEST_ENSURE_OP(divideAndRoundUp(2, -3), ==, 0);
			TEST_ENSURE_OP(divideAndRoundUp(3, -3), ==, -1);
			TEST_ENSURE_OP(divideAndRoundUp(4, -3), ==, -1);
			TEST_ENSURE_OP(divideAndRoundUp(5, -3), ==, -1);
			TEST_ENSURE_OP(divideAndRoundUp(6, -3), ==, -2);
			TEST_ENSURE_OP(divideAndRoundUp(7, -3), ==, -2);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTests()
	{
		testRunner().add("divideAndRoundUp", test);
	}

	CallFunction run(addTests);

}
