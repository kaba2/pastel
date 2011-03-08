// Description: Testing for string algorithms
// DocumentationOf: string_algorithms.h

#include "pastelsystest.h"

#include "pastel/sys/string_algorithms.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/isnan.h"

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&sysTestReport())
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
			TEST_ENSURE(stringToReal("inf") == infinity<real>());
			TEST_ENSURE(stringToReal("+inf") == infinity<real>());
			TEST_ENSURE(stringToReal("infinity") == infinity<real>());
			TEST_ENSURE(stringToReal("+infinity") == infinity<real>());
			TEST_ENSURE(stringToReal("-inf") == -infinity<real>());
			TEST_ENSURE(stringToReal("-infinity") == -infinity<real>());
		}

		void testRealToString()
		{
			TEST_ENSURE(realToString(infinity<real>()) == "inf");
			TEST_ENSURE(realToString(-infinity<real>()) == "-inf");
			TEST_ENSURE(realToString(nan<real>()) == "nan");
		}

	};

	void testStringTools()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		sysTestList().add("string_algorithms", testStringTools);
	}

	CallFunction run(addTest);

}
