// Description: Testing for bit masks
// DocumentationOf: bitmask.h

#include "test/test_init.h"

#include <pastel/sys/bit/bitmask.h>

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			test();
		}

		void test()
		{
			REQUIRE(singleBitMask<uint8>(0) == 1);
			REQUIRE(singleBitMask<uint8>(1) == 2);
			REQUIRE(singleBitMask<uint8>(2) == 4);
			REQUIRE(singleBitMask<uint8>(3) == 8);
			REQUIRE(singleBitMask<uint8>(4) == 16);
			REQUIRE(singleBitMask<uint8>(5) == 32);
			REQUIRE(singleBitMask<uint8>(6) == 64);
			REQUIRE(singleBitMask<uint8>(7) == 128);

			REQUIRE(bitMask<uint8>(0) == 0x00);
			REQUIRE(bitMask<uint8>(1) == 0x01);
			REQUIRE(bitMask<uint8>(2) == 0x03);
			REQUIRE(bitMask<uint8>(3) == 0x07);
			REQUIRE(bitMask<uint8>(4) == 0x0F);
			REQUIRE(bitMask<uint8>(5) == 0x1F);
			REQUIRE(bitMask<uint8>(6) == 0x3F);
			REQUIRE(bitMask<uint8>(7) == 0x7F);
			REQUIRE(bitMask<uint8>(8) == 0xFF);

			REQUIRE(bitMask<uint8>(0, 0) == 0x00);
			REQUIRE(bitMask<uint8>(0, 1) == 0x01);
			REQUIRE(bitMask<uint8>(0, 2) == 0x03);
			REQUIRE(bitMask<uint8>(0, 3) == 0x07);
			REQUIRE(bitMask<uint8>(0, 4) == 0x0F);
			REQUIRE(bitMask<uint8>(0, 5) == 0x1F);
			REQUIRE(bitMask<uint8>(0, 6) == 0x3F);
			REQUIRE(bitMask<uint8>(0, 7) == 0x7F);
			REQUIRE(bitMask<uint8>(0, 8) == 0xFF);

			REQUIRE(bitMask<uint8>(1, 8) == 0xFE);
			REQUIRE(bitMask<uint8>(2, 8) == 0xFC);
			REQUIRE(bitMask<uint8>(3, 8) == 0xF8);
			REQUIRE(bitMask<uint8>(4, 8) == 0xF0);
			REQUIRE(bitMask<uint8>(5, 8) == 0xE0);
			REQUIRE(bitMask<uint8>(6, 8) == 0xC0);
			REQUIRE(bitMask<uint8>(7, 8) == 0x80);
			REQUIRE(bitMask<uint8>(8, 8) == 0x00);
		}
	};

	TEST_CASE("bitMask", "[bitMask]")
	{
	}

}
