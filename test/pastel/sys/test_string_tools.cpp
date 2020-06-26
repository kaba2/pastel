// Description: Testing for string algorithms
// DocumentationOf: string_algorithms.h

#include "test/test_init.h"

#include "pastel/sys/string.h"
#include "pastel/sys/math/constants.h"

TEST_CASE("stringToReal (string_algorithms)")
{
	REQUIRE(isNan(stringToReal("nan")));
	REQUIRE(isNan(stringToReal("+nan")));
	REQUIRE(isNan(stringToReal("-nan")));
	REQUIRE(stringToReal("inf") == (dreal)Infinity());
	REQUIRE(stringToReal("+inf") == (dreal)Infinity());
	REQUIRE(stringToReal("-inf") == -(dreal)Infinity());
	REQUIRE(stringToReal("123") == 123);
	REQUIRE(stringToReal("-123") == -123);
	REQUIRE(stringToReal("+123") == +123);
	REQUIRE(stringToReal("1.0") == 1.0);
	REQUIRE(stringToReal("1.01") == 1.01);
	REQUIRE(stringToReal("1.01e5") == 1.01e5);
	REQUIRE(stringToReal("1.01e-5") == 1.01e-5);
	REQUIRE(stringToReal("1.01e+5") == 1.01e+5);
}

TEST_CASE("realToString (string_algorithms)")
{
	REQUIRE(realToString((dreal)Infinity()) == "inf");
	REQUIRE(realToString(-(dreal)Infinity()) == "-inf");
	REQUIRE(realToString((dreal)Nan()) == "nan");
	REQUIRE(realToString(123) == "123");
	REQUIRE(realToString(-123) == "-123");
}
