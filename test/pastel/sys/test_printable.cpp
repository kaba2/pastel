// Description: Testing for the printable concept
// Documentation: unit_testing.txt

#include "test/test_init.h"

#include <pastel/sys/printable.h>
#include <pastel/sys/math/constants.h>

TEST_CASE("printable (printable)")
{
	PASTEL_CONCEPT_CHECK(integer, Printable_Concept);

	enum class A { a = 113 };

	REQUIRE(abs(stringAsReal<float>("3.34897") - 3.34897) <= 0.000005);
	REQUIRE(abs(stringAsReal<float>("5.09865") - 5.09865) <= 0.000005);
	REQUIRE(abs(stringAsReal<double>("5.12345678901234567890") - 5.12345678901234567890) == 0);

	REQUIRE(asString(A::a) == "113");
	REQUIRE(asString(113) == "113");
	REQUIRE(asString((double)113) == "113");
	REQUIRE(asString((float)113) == "113");
	REQUIRE(asString('a') == "a");
	REQUIRE(asString("test") == "test");
	REQUIRE(asString(std::string("test")) == "test");
	REQUIRE(asString((short)113) == "113");
	REQUIRE(asString((ushort)113) == "113");
	REQUIRE(asString((int)113) == "113");
	REQUIRE(asString((uint)113) == "113");
	REQUIRE(asString((long)113) == "113");
	REQUIRE(asString((ulong)113) == "113");

	// FIX: What should happen here?
	//char testArray[] = { 't', 'e', 's', 't'};
	//REQUIRE(asString(testArray) == "test");
}

