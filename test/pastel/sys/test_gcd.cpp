// Description: Testing for gcd
// DocumentationOf: gcd.h

#include "test/test_init.h"

#include "pastel/sys/integer/multi_integer.h"
#include "pastel/sys/integer/gcd.h"

namespace
{

	template <typename Integer>
	void testCase()
	{
		REQUIRE(gcd(Integer(1), Integer(5)) == Integer(1));
		REQUIRE(gcd(Integer(1), Integer(3455)) == Integer(1));
		REQUIRE(gcd(Integer(1), Integer(2235)) == Integer(1));
		REQUIRE(gcd(Integer(1), Integer(2344)) == Integer(1));

		REQUIRE(gcd(Integer(0), Integer(2344)) == Integer(2344));
		REQUIRE(gcd(Integer(0), Integer(23455)) == Integer(23455));
		REQUIRE(gcd(Integer(0), Integer(865)) == Integer(865));
		REQUIRE(gcd(Integer(0), Integer(5487)) == Integer(5487));

		REQUIRE(gcd(Integer(13), Integer(7)) == Integer(1));
		REQUIRE(gcd(Integer(5), Integer(23)) == Integer(1));
		REQUIRE(gcd(Integer(7), Integer(11)) == Integer(1));
		REQUIRE(gcd(Integer(3), Integer(2)) == Integer(1));

		REQUIRE(gcd(Integer(5 * 1234), Integer(7 * 1234)) == Integer(1234));
		REQUIRE(gcd(Integer(2), Integer(4)) == Integer(2));

		REQUIRE(gcd(Integer(16), Integer(256)) == Integer(16));
		REQUIRE(gcd(Integer(128), Integer(256)) == Integer(128));
		REQUIRE(gcd(Integer(256), Integer(128)) == Integer(128));
	}

}

TEST_CASE("Gcd (Gcd)")
{
	testCase<int16>();
	testCase<int32>();
	testCase<integer>();
	testCase<Signed_Integer<16>>();
	testCase<Signed_Integer<32>>();
}
