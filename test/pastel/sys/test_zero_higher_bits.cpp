// Description: Testing for zeroing higher bits
// DocumentationOf: zero_higher_bits.h

#include "test/test_init.h"

#include <pastel/sys/bit/zero_higher_bits.h>

TEST_CASE("zeroHigherBits (zeroHigherBits)")
{
	auto test = [&](uint32 that, uint32 correct)
	{
		return zeroHigherBits(that) == correct;
	};

	REQUIRE(test(0x00000000, 0x00000000));
	REQUIRE(test(0x80000000, 0x80000000));
	REQUIRE(test(0x40000000, 0x40000000));
	REQUIRE(test(0x20000000, 0x20000000));
	REQUIRE(test(0x10000000, 0x10000000));
	REQUIRE(test(0x08000000, 0x08000000));
	REQUIRE(test(0x04000000, 0x04000000));
	REQUIRE(test(0x02000000, 0x02000000));
	REQUIRE(test(0x01000000, 0x01000000));
	REQUIRE(test(0x00800000, 0x00800000));
	REQUIRE(test(0x00400000, 0x00400000));
	REQUIRE(test(0x00200000, 0x00200000));
	REQUIRE(test(0x00100000, 0x00100000));
	REQUIRE(test(0x00080000, 0x00080000));
	REQUIRE(test(0x00040000, 0x00040000));
	REQUIRE(test(0x00020000, 0x00020000));
	REQUIRE(test(0x00010000, 0x00010000));
	REQUIRE(test(0x00008000, 0x00008000));
	REQUIRE(test(0x00004000, 0x00004000));
	REQUIRE(test(0x00002000, 0x00002000));
	REQUIRE(test(0x00001000, 0x00001000));
	REQUIRE(test(0x00000800, 0x00000800));
	REQUIRE(test(0x00000400, 0x00000400));
	REQUIRE(test(0x00000200, 0x00000200));
	REQUIRE(test(0x00000100, 0x00000100));
	REQUIRE(test(0x00000080, 0x00000080));
	REQUIRE(test(0x00000040, 0x00000040));
	REQUIRE(test(0x00000020, 0x00000020));
	REQUIRE(test(0x00000010, 0x00000010));
	REQUIRE(test(0x00000008, 0x00000008));
	REQUIRE(test(0x00000004, 0x00000004));
	REQUIRE(test(0x00000002, 0x00000002));
	REQUIRE(test(0x00000001, 0x00000001));
	REQUIRE(test(0x87654321, 0x00000001));
	REQUIRE(test(0x87654322, 0x00000002));
	REQUIRE(test(0x87654323, 0x00000001));
	REQUIRE(test(0x87654324, 0x00000004));
	REQUIRE(test(0x87654325, 0x00000001));
	REQUIRE(test(0x87654326, 0x00000002));
	REQUIRE(test(0x87654327, 0x00000001));
	REQUIRE(test(0x87654328, 0x00000008));
	REQUIRE(test(0x87654329, 0x00000001));
	REQUIRE(test(0x8765432A, 0x00000002));
	REQUIRE(test(0x8765432B, 0x00000001));
	REQUIRE(test(0x8765432C, 0x00000004));
	REQUIRE(test(0x8765432D, 0x00000001));
	REQUIRE(test(0x8765432E, 0x00000002));
	REQUIRE(test(0x8765432F, 0x00000001));

	REQUIRE(test(0x0010, 0x0010));
	REQUIRE(test(0x0100, 0x0100));
	REQUIRE(test(0x1000, 0x1000));

	REQUIRE(test(0x3501, 0x0001));
	REQUIRE(test(0x3510, 0x0010));
	REQUIRE(test(0x3100, 0x0100));
}

