// Description: Testing for two's complement
// DocumentationOf: twos_complement.h

#include "test/test_init.h"

#include <pastel/sys/bit/twos_complement.h>

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			testSignedToTwosComplement();
			testTwosComplementToSigned();
			testInverse();
			testOtherInverse();
			testShifts();
		}

		void testSignedToTwosComplement()
		{
			REQUIRE(signedToTwosComplement((int8)+0x00) == (uint8)0x00);
			REQUIRE(signedToTwosComplement((int8)-0x01) == (uint8)0xFF);
			REQUIRE(signedToTwosComplement((int8)-0x02) == (uint8)0xFE);
			REQUIRE(signedToTwosComplement((int8)-0x03) == (uint8)0xFD);
			REQUIRE(signedToTwosComplement((int8)-0x04) == (uint8)0xFC);
			REQUIRE(signedToTwosComplement((int8)-0x05) == (uint8)0xFB);
			REQUIRE(signedToTwosComplement((int8)-0x06) == (uint8)0xFA);
			REQUIRE(signedToTwosComplement((int8)-0x07) == (uint8)0xF9);
			REQUIRE(signedToTwosComplement((int8)-0x08) == (uint8)0xF8);
			REQUIRE(signedToTwosComplement((int8)-0x09) == (uint8)0xF7);
			REQUIRE(signedToTwosComplement((int8)-0x0A) == (uint8)0xF6);
			REQUIRE(signedToTwosComplement((int8)-0x0B) == (uint8)0xF5);
			REQUIRE(signedToTwosComplement((int8)-0x0C) == (uint8)0xF4);
			REQUIRE(signedToTwosComplement((int8)-0x0D) == (uint8)0xF3);
			REQUIRE(signedToTwosComplement((int8)-0x0E) == (uint8)0xF2);
			REQUIRE(signedToTwosComplement((int8)-0x0F) == (uint8)0xF1);
			REQUIRE(signedToTwosComplement((int8)-0x10) == (uint8)0xF0);
			REQUIRE(signedToTwosComplement((int8)-0x11) == (uint8)0xEF);
			REQUIRE(signedToTwosComplement((int8)-0x12) == (uint8)0xEE);
			REQUIRE(signedToTwosComplement((int8)-0x13) == (uint8)0xED);
			REQUIRE(signedToTwosComplement((int8)-0x14) == (uint8)0xEC);
			REQUIRE(signedToTwosComplement((int8)-0x15) == (uint8)0xEB);
			REQUIRE(signedToTwosComplement((int8)-0x16) == (uint8)0xEA);
			REQUIRE(signedToTwosComplement((int8)-0x17) == (uint8)0xE9);
			REQUIRE(signedToTwosComplement((int8)-0x18) == (uint8)0xE8);
			REQUIRE(signedToTwosComplement((int8)-0x19) == (uint8)0xE7);
			REQUIRE(signedToTwosComplement((int8)-0x1A) == (uint8)0xE6);
			REQUIRE(signedToTwosComplement((int8)-0x1B) == (uint8)0xE5);
			REQUIRE(signedToTwosComplement((int8)-0x1C) == (uint8)0xE4);
			REQUIRE(signedToTwosComplement((int8)-0x1D) == (uint8)0xE3);
			REQUIRE(signedToTwosComplement((int8)-0x1E) == (uint8)0xE2);
			REQUIRE(signedToTwosComplement((int8)-0x1F) == (uint8)0xE1);
			REQUIRE(signedToTwosComplement((int8)-0x20) == (uint8)0xE0);

			REQUIRE(signedToTwosComplement((int8)+0x00) == (uint8)0x00);
			REQUIRE(signedToTwosComplement((int8)+0x01) == (uint8)0x01);
			REQUIRE(signedToTwosComplement((int8)+0x02) == (uint8)0x02);
			REQUIRE(signedToTwosComplement((int8)+0x03) == (uint8)0x03);
			REQUIRE(signedToTwosComplement((int8)+0x04) == (uint8)0x04);
			REQUIRE(signedToTwosComplement((int8)+0x05) == (uint8)0x05);
			REQUIRE(signedToTwosComplement((int8)+0x06) == (uint8)0x06);
			REQUIRE(signedToTwosComplement((int8)+0x07) == (uint8)0x07);
			REQUIRE(signedToTwosComplement((int8)+0x08) == (uint8)0x08);
			REQUIRE(signedToTwosComplement((int8)+0x09) == (uint8)0x09);
			REQUIRE(signedToTwosComplement((int8)+0x0A) == (uint8)0x0A);
			REQUIRE(signedToTwosComplement((int8)+0x0B) == (uint8)0x0B);
			REQUIRE(signedToTwosComplement((int8)+0x0C) == (uint8)0x0C);
			REQUIRE(signedToTwosComplement((int8)+0x0D) == (uint8)0x0D);
			REQUIRE(signedToTwosComplement((int8)+0x0E) == (uint8)0x0E);
			REQUIRE(signedToTwosComplement((int8)+0x0F) == (uint8)0x0F);
			REQUIRE(signedToTwosComplement((int8)+0x10) == (uint8)0x10);
			REQUIRE(signedToTwosComplement((int8)+0x11) == (uint8)0x11);
			REQUIRE(signedToTwosComplement((int8)+0x12) == (uint8)0x12);
			REQUIRE(signedToTwosComplement((int8)+0x13) == (uint8)0x13);
			REQUIRE(signedToTwosComplement((int8)+0x14) == (uint8)0x14);
			REQUIRE(signedToTwosComplement((int8)+0x15) == (uint8)0x15);
			REQUIRE(signedToTwosComplement((int8)+0x16) == (uint8)0x16);
			REQUIRE(signedToTwosComplement((int8)+0x17) == (uint8)0x17);
			REQUIRE(signedToTwosComplement((int8)+0x18) == (uint8)0x18);
			REQUIRE(signedToTwosComplement((int8)+0x19) == (uint8)0x19);
			REQUIRE(signedToTwosComplement((int8)+0x1A) == (uint8)0x1A);
			REQUIRE(signedToTwosComplement((int8)+0x1B) == (uint8)0x1B);
			REQUIRE(signedToTwosComplement((int8)+0x1C) == (uint8)0x1C);
			REQUIRE(signedToTwosComplement((int8)+0x1D) == (uint8)0x1D);
			REQUIRE(signedToTwosComplement((int8)+0x1E) == (uint8)0x1E);
			REQUIRE(signedToTwosComplement((int8)+0x1F) == (uint8)0x1F);
			REQUIRE(signedToTwosComplement((int8)+0x20) == (uint8)0x20);
		}

		void testTwosComplementToSigned()
		{
			REQUIRE(twosComplementToSigned((uint8)0x00) == (int8)+0x00);
			REQUIRE(twosComplementToSigned((uint8)0xFF) == (int8)-0x01);
			REQUIRE(twosComplementToSigned((uint8)0xFE) == (int8)-0x02);
			REQUIRE(twosComplementToSigned((uint8)0xFD) == (int8)-0x03);
			REQUIRE(twosComplementToSigned((uint8)0xFC) == (int8)-0x04);
			REQUIRE(twosComplementToSigned((uint8)0xFB) == (int8)-0x05);
			REQUIRE(twosComplementToSigned((uint8)0xFA) == (int8)-0x06);
			REQUIRE(twosComplementToSigned((uint8)0xF9) == (int8)-0x07);
			REQUIRE(twosComplementToSigned((uint8)0xF8) == (int8)-0x08);
			REQUIRE(twosComplementToSigned((uint8)0xF7) == (int8)-0x09);
			REQUIRE(twosComplementToSigned((uint8)0xF6) == (int8)-0x0A);
			REQUIRE(twosComplementToSigned((uint8)0xF5) == (int8)-0x0B);
			REQUIRE(twosComplementToSigned((uint8)0xF4) == (int8)-0x0C);
			REQUIRE(twosComplementToSigned((uint8)0xF3) == (int8)-0x0D);
			REQUIRE(twosComplementToSigned((uint8)0xF2) == (int8)-0x0E);
			REQUIRE(twosComplementToSigned((uint8)0xF1) == (int8)-0x0F);
			REQUIRE(twosComplementToSigned((uint8)0xF0) == (int8)-0x10);
			REQUIRE(twosComplementToSigned((uint8)0xEF) == (int8)-0x11);
			REQUIRE(twosComplementToSigned((uint8)0xEE) == (int8)-0x12);
			REQUIRE(twosComplementToSigned((uint8)0xED) == (int8)-0x13);
			REQUIRE(twosComplementToSigned((uint8)0xEC) == (int8)-0x14);
			REQUIRE(twosComplementToSigned((uint8)0xEB) == (int8)-0x15);
			REQUIRE(twosComplementToSigned((uint8)0xEA) == (int8)-0x16);
			REQUIRE(twosComplementToSigned((uint8)0xE9) == (int8)-0x17);
			REQUIRE(twosComplementToSigned((uint8)0xE8) == (int8)-0x18);
			REQUIRE(twosComplementToSigned((uint8)0xE7) == (int8)-0x19);
			REQUIRE(twosComplementToSigned((uint8)0xE6) == (int8)-0x1A);
			REQUIRE(twosComplementToSigned((uint8)0xE5) == (int8)-0x1B);
			REQUIRE(twosComplementToSigned((uint8)0xE4) == (int8)-0x1C);
			REQUIRE(twosComplementToSigned((uint8)0xE3) == (int8)-0x1D);
			REQUIRE(twosComplementToSigned((uint8)0xE2) == (int8)-0x1E);
			REQUIRE(twosComplementToSigned((uint8)0xE1) == (int8)-0x1F);
			REQUIRE(twosComplementToSigned((uint8)0xE0) == (int8)-0x20);

			REQUIRE(twosComplementToSigned((uint8)0x00) == (int8)+0x00);
			REQUIRE(twosComplementToSigned((uint8)0x01) == (int8)+0x01);
			REQUIRE(twosComplementToSigned((uint8)0x02) == (int8)+0x02);
			REQUIRE(twosComplementToSigned((uint8)0x03) == (int8)+0x03);
			REQUIRE(twosComplementToSigned((uint8)0x04) == (int8)+0x04);
			REQUIRE(twosComplementToSigned((uint8)0x05) == (int8)+0x05);
			REQUIRE(twosComplementToSigned((uint8)0x06) == (int8)+0x06);
			REQUIRE(twosComplementToSigned((uint8)0x07) == (int8)+0x07);
			REQUIRE(twosComplementToSigned((uint8)0x08) == (int8)+0x08);
			REQUIRE(twosComplementToSigned((uint8)0x09) == (int8)+0x09);
			REQUIRE(twosComplementToSigned((uint8)0x0A) == (int8)+0x0A);
			REQUIRE(twosComplementToSigned((uint8)0x0B) == (int8)+0x0B);
			REQUIRE(twosComplementToSigned((uint8)0x0C) == (int8)+0x0C);
			REQUIRE(twosComplementToSigned((uint8)0x0D) == (int8)+0x0D);
			REQUIRE(twosComplementToSigned((uint8)0x0E) == (int8)+0x0E);
			REQUIRE(twosComplementToSigned((uint8)0x0F) == (int8)+0x0F);
			REQUIRE(twosComplementToSigned((uint8)0x10) == (int8)+0x10);
			REQUIRE(twosComplementToSigned((uint8)0x11) == (int8)+0x11);
			REQUIRE(twosComplementToSigned((uint8)0x12) == (int8)+0x12);
			REQUIRE(twosComplementToSigned((uint8)0x13) == (int8)+0x13);
			REQUIRE(twosComplementToSigned((uint8)0x14) == (int8)+0x14);
			REQUIRE(twosComplementToSigned((uint8)0x15) == (int8)+0x15);
			REQUIRE(twosComplementToSigned((uint8)0x16) == (int8)+0x16);
			REQUIRE(twosComplementToSigned((uint8)0x17) == (int8)+0x17);
			REQUIRE(twosComplementToSigned((uint8)0x18) == (int8)+0x18);
			REQUIRE(twosComplementToSigned((uint8)0x19) == (int8)+0x19);
			REQUIRE(twosComplementToSigned((uint8)0x1A) == (int8)+0x1A);
			REQUIRE(twosComplementToSigned((uint8)0x1B) == (int8)+0x1B);
			REQUIRE(twosComplementToSigned((uint8)0x1C) == (int8)+0x1C);
			REQUIRE(twosComplementToSigned((uint8)0x1D) == (int8)+0x1D);
			REQUIRE(twosComplementToSigned((uint8)0x1E) == (int8)+0x1E);
			REQUIRE(twosComplementToSigned((uint8)0x1F) == (int8)+0x1F);
			REQUIRE(twosComplementToSigned((uint8)0x20) == (int8)+0x20);
		}

		void testInverse()
		{
			for (uint8 i = 0;i < 255;++i)
			{
				int8 a = twosComplementToSigned(i);
				uint8 j = signedToTwosComplement(a);
				REQUIRE(i == j);
			}

			int8 a = twosComplementToSigned((uint8)255);
			uint8 j = signedToTwosComplement(a);
			REQUIRE((uint8)255 == j);
		}

		void testOtherInverse()
		{
			// Note that -128 is not included;
			// the standard only guarantees the range [-127, +127],
			// corresponding to the sign-magnitude and the
			// one's complement representations.
			for (int8 i = -127; i < 127; ++i)
			{
				uint8 a = signedToTwosComplement(i);
				int8 j = twosComplementToSigned(a);
				REQUIRE(i == j);
			}

			uint8 a = signedToTwosComplement((int8)127);
			int8 j = twosComplementToSigned(a);
			REQUIRE((int8)127 == j);
		}

		void testShifts()
		{
			auto F = [](uint8 a, integer i)
			{
				return arithmeticShiftRight(a, i);
			};

			REQUIRE(F(0xFF, 0) == 0xFF);
			REQUIRE(F(0xFF, 1) == 0xFF);
			REQUIRE(F(0xFF, 2) == 0xFF);
			REQUIRE(F(0xFF, 3) == 0xFF);
			REQUIRE(F(0xFF, 4) == 0xFF);
			REQUIRE(F(0xFF, 5) == 0xFF);
			REQUIRE(F(0xFF, 6) == 0xFF);
			REQUIRE(F(0xFF, 7) == 0xFF);
			REQUIRE(F(0xFF, 8) == 0xFF);
			REQUIRE(F(0xFF, 9) == 0xFF);

			REQUIRE(F(0x38, 0) == 0x38);
			REQUIRE(F(0x38, 1) == 0x1C);
			REQUIRE(F(0x38, 2) == 0x0E);
			REQUIRE(F(0x38, 3) == 0x07);
			REQUIRE(F(0x38, 4) == 0x03);
			REQUIRE(F(0x38, 5) == 0x01);
			REQUIRE(F(0x38, 6) == 0x00);
			REQUIRE(F(0x38, 7) == 0x00);
			REQUIRE(F(0x38, 8) == 0x00);
			REQUIRE(F(0x38, 9) == 0x00);

			REQUIRE(F(0x00, 0) == 0x00);
			REQUIRE(F(0x00, 1) == 0x00);
			REQUIRE(F(0x00, 2) == 0x00);
			REQUIRE(F(0x00, 3) == 0x00);
			REQUIRE(F(0x00, 4) == 0x00);
			REQUIRE(F(0x00, 5) == 0x00);
			REQUIRE(F(0x00, 6) == 0x00);
			REQUIRE(F(0x00, 7) == 0x00);
			REQUIRE(F(0x00, 8) == 0x00);
			REQUIRE(F(0x00, 9) == 0x00);
		}
	};

	TEST_CASE("twos_complement", "[twos_complement]")
	{
	}

}
