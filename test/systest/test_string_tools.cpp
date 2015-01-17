// Description: Testing for string algorithms
// DocumentationOf: string_algorithms.h

#include "test_pastelsys.h"

#include "pastel/sys/string.h"
#include "pastel/sys/math/constants.h"

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
			testStringToReal();
			testRealToString();
		}

		void testStringToReal()
		{
			TEST_ENSURE(isNan(stringToReal("nan")));
			TEST_ENSURE(isNan(stringToReal("+nan")));
			TEST_ENSURE(isNan(stringToReal("-nan")));
			TEST_ENSURE_OP(stringToReal("inf"), ==, infinity<real>());
			TEST_ENSURE_OP(stringToReal("+inf"), ==, infinity<real>());
			TEST_ENSURE_OP(stringToReal("-inf"), ==, -infinity<real>());
			TEST_ENSURE_OP(stringToReal("123"), ==, 123);
			TEST_ENSURE_OP(stringToReal("-123"), ==, -123);
			TEST_ENSURE_OP(stringToReal("+123"), ==, +123);
			TEST_ENSURE_OP(stringToReal("1.0"), ==, 1.0);
			TEST_ENSURE_OP(stringToReal("1.01"), ==, 1.01);
			TEST_ENSURE_OP(stringToReal("1.01e5"), ==, 1.01e5);
			TEST_ENSURE_OP(stringToReal("1.01e-5"), ==, 1.01e-5);
			TEST_ENSURE_OP(stringToReal("1.01e+5"), ==, 1.01e+5);
		}

		void testRealToString()
		{
			TEST_ENSURE(realToString(infinity<real>()) == "inf");
			TEST_ENSURE(realToString(-infinity<real>()) == "-inf");
			TEST_ENSURE(realToString(nan<real>()) == "nan");
			TEST_ENSURE(realToString(123) == "123");
			TEST_ENSURE(realToString(-123) == "-123");
		}

	};

	void testStringTools()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("string_algorithms", testStringTools);
	}

	CallFunction run(addTest);

}
