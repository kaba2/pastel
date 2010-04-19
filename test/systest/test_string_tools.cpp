#include "pastelsystest.h"

#include "pastel/sys/string_tools.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/stdext_isnan.h"

using namespace Pastel;

namespace
{

	class StringTools_Test
		: public TestSuite
	{
	public:
		StringTools_Test()
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
			TEST_ENSURE(StdExt::isNan(stringToReal("nan")));
			TEST_ENSURE(StdExt::isNan(stringToReal("+nan")));
			TEST_ENSURE(StdExt::isNan(stringToReal("-nan")));
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
		StringTools_Test test;
		test.run();
	}

	void addTest()
	{
		sysTestList().add("string_tools", testStringTools);
	}

	CallFunction run(addTest);

}
