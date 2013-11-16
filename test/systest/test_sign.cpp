// Description: Testing for sign
// DocumentationOf: sign.h

#include "test_pastelsys.h"

#include "pastel/sys/sign.h"

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
			TEST_ENSURE_OP(sign(-100), ==, -1);
			TEST_ENSURE_OP(sign(100), ==, 1);

			TEST_ENSURE_OP(sign(-1), ==, -1);
			TEST_ENSURE_OP(sign(0), ==, 0);
			TEST_ENSURE_OP(sign(1), ==, 1);

			TEST_ENSURE_OP(sign(-1.0), ==, -1);
			TEST_ENSURE_OP(sign(0.0), ==, 0);
			TEST_ENSURE_OP(sign(1.0), ==, 1);

			TEST_ENSURE_OP(sign(-1.0f), ==, -1);
			TEST_ENSURE_OP(sign(0.0f), ==, 0);
			TEST_ENSURE_OP(sign(1.0f), ==, 1);

			TEST_ENSURE_OP(sign(infinity<float>()), ==, 1);
			TEST_ENSURE_OP(sign(-infinity<float>()), ==, -1);
			TEST_ENSURE_OP(sign(nan<float>()), ==, 0);
			TEST_ENSURE_OP(sign(-0.0f), ==, 0);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTests()
	{
		testRunner().add("sign", test);
	}

	CallFunction run(addTests);

}
