// Description: Testing for IEEE floating-point numbers
// DocumentationOf: ieee_float.h

#include "test/test_init.h"

#include "pastel/sys/bit/bitmask.h"
#include "pastel/sys/real/ieee_float.h"
#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/logging.h"

namespace
{

	template <typename Type>
	void testCase()
	{
		auto t = [](bool negative, integer exponent, uint64 mantissa, Type correct)
		{
			ScientificNotation s{ negative, exponent, mantissa };

			return (asIeeeFloat<Type>(s) == correct) &&
				(asIeeeFloat<Type>(asScientific(correct)) == correct);
		};

		uint64 oneHalf = singleBitMask<uint64>(SizeInBits<uint64>::value - 1);

		REQUIRE(t(false, 0, 0, 1));
		REQUIRE(t(false, 1, 0, 2));
		REQUIRE(t(false, 2, 0, 4));
		REQUIRE(t(false, 3, 0, 8));
		REQUIRE(t(false, 4, 0, 16));

		REQUIRE(t(false, -1, 0, (Type)1 / 2));
		REQUIRE(t(false, -2, 0, (Type)1 / 4));
		REQUIRE(t(false, -3, 0, (Type)1 / 8));
		REQUIRE(t(false, -4, 0, (Type)1 / 16));

		REQUIRE(t(true, 0, 0, -1));
		REQUIRE(t(true, 1, 0, -2));
		REQUIRE(t(true, 2, 0, -4));
		REQUIRE(t(true, 3, 0, -8));
		REQUIRE(t(true, 4, 0, -16));

		REQUIRE(t(true, -1, 0, (Type)-1 / 2));
		REQUIRE(t(true, -2, 0, (Type)-1 / 4));
		REQUIRE(t(true, -3, 0, (Type)-1 / 8));
		REQUIRE(t(true, -4, 0, (Type)-1 / 16));

		REQUIRE(t(false, 0, oneHalf, (Type)1 * 1.5));
		REQUIRE(t(false, 1, oneHalf, (Type)2 * 1.5));
		REQUIRE(t(false, 2, oneHalf, (Type)4 * 1.5));
		REQUIRE(t(false, 3, oneHalf, (Type)8 * 1.5));
		REQUIRE(t(false, 4, oneHalf, (Type)16 * 1.5));

		REQUIRE(t(false, -1, oneHalf, (Type)1.5 / 2));
		REQUIRE(t(false, -2, oneHalf, (Type)1.5 / 4));
		REQUIRE(t(false, -3, oneHalf, (Type)1.5 / 8));
		REQUIRE(t(false, -4, oneHalf, (Type)1.5 / 16));

		REQUIRE(t(true, 0, oneHalf, (Type)-1 * 1.5));
		REQUIRE(t(true, 1, oneHalf, (Type)-2 * 1.5));
		REQUIRE(t(true, 2, oneHalf, (Type)-4 * 1.5));
		REQUIRE(t(true, 3, oneHalf, (Type)-8 * 1.5));
		REQUIRE(t(true, 4, oneHalf, (Type)-16 * 1.5));

		REQUIRE(t(true, -1, oneHalf, (Type)-1.5 / 2));
		REQUIRE(t(true, -2, oneHalf, (Type)-1.5 / 4));
		REQUIRE(t(true, -3, oneHalf, (Type)-1.5 / 8));
		REQUIRE(t(true, -4, oneHalf, (Type)-1.5 / 16));

		REQUIRE(t(false, 1000000, 0, infinity<Type>()));
		REQUIRE(t(true, 1000000, 0, -infinity<Type>()));

		REQUIRE(t(false, -1000000, 0, 0));
		REQUIRE(t(true, -1000000, 0, 0));

		REQUIRE(t(false, 1000000, oneHalf, infinity<Type>()));
		REQUIRE(t(true, 1000000, oneHalf, -infinity<Type>()));

		REQUIRE(t(false, -1000000, oneHalf, 0));
		REQUIRE(t(true, -1000000, oneHalf, 0));
	}

}

TEST_CASE("ieee_float (ieee_float)")
{
	testCase<float>();
	testCase<double>();
}
