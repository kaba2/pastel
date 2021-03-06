// Description: Testing for native reals
// DocumentationOf: native_real.h

#include "test/test_init.h"

#include <pastel/sys/real/real_concept.h>

namespace
{

	template <typename Type>
	void testCase()
	{
		PASTEL_CONCEPT_CHECK(Type, Real_Concept);

		REQUIRE(isInfinity((Type)Infinity()));
		REQUIRE(isMinusInfinity(-(Type)Infinity()));
		REQUIRE(isNan((Type)Nan()));
		REQUIRE(inverse((Type)5) == 1 / (Type)5);

		REQUIRE(negative((Type)-1));
		REQUIRE(!negative((Type)1));
		REQUIRE(!negative((Type)0));

		REQUIRE(positive((Type)1));
		REQUIRE(!positive((Type)-1));
		REQUIRE(!positive((Type)0));

		REQUIRE(zero((Type)0));
		REQUIRE(zero(-(Type)0));
		REQUIRE(!zero((Type)1));
		REQUIRE(!zero((Type)-1));

		REQUIRE(floor((Type)0.5) == 0);
		REQUIRE(floor((Type)-0.5) == -1);

		REQUIRE(ceil((Type)0.5) == 1);
		REQUIRE(ceil((Type)-0.5) == 0);
	}

}

TEST_CASE("native_real (native_real)")
{
	testCase<float>();
	testCase<double>();
	testCase<long double>();
}