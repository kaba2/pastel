// Description: Testing for string algorithms
// DocumentationOf: string_algorithms.h

#include "test/test_init.h"

#include "pastel/sys/string.h"
#include "pastel/sys/math/constants.h"

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			testStringToReal();
			testRealToString();
		}

		void testStringToReal()
		{
			REQUIRE(isNan(stringToReal("nan")));
			REQUIRE(isNan(stringToReal("+nan")));
			REQUIRE(isNan(stringToReal("-nan")));
			REQUIRE(stringToReal("inf") == infinity<real>());
			REQUIRE(stringToReal("+inf") == infinity<real>());
			REQUIRE(stringToReal("-inf") == -infinity<real>());
			REQUIRE(stringToReal("123") == 123);
			REQUIRE(stringToReal("-123") == -123);
			REQUIRE(stringToReal("+123") == +123);
			REQUIRE(stringToReal("1.0") == 1.0);
			REQUIRE(stringToReal("1.01") == 1.01);
			REQUIRE(stringToReal("1.01e5") == 1.01e5);
			REQUIRE(stringToReal("1.01e-5") == 1.01e-5);
			REQUIRE(stringToReal("1.01e+5") == 1.01e+5);
		}

		void testRealToString()
		{
			REQUIRE(realToString(infinity<real>()) == "inf");
			REQUIRE(realToString(-infinity<real>()) == "-inf");
			REQUIRE(realToString(nan<real>()) == "nan");
			REQUIRE(realToString(123) == "123");
			REQUIRE(realToString(-123) == "-123");
		}

	};

	TEST_CASE("string_algorithms", "[string_algorithms]")
	{
	}

}
