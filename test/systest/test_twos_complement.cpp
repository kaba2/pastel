// Description: Testing for two's complement
// DocumentationOf: twos_complement.h

#include "test_pastelsys.h"

#include <pastel/sys/twos_complement.h>

using namespace Pastel;
using namespace std;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			testSignedToTwosComplement();
			testTwosComplementToSigned();
			testInverse();
			testOtherInverse();
		}

		void testSignedToTwosComplement()
		{
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x00), ==, (uint8)0x00);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x01), ==, (uint8)0xFF);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x02), ==, (uint8)0xFE);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x03), ==, (uint8)0xFD);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x04), ==, (uint8)0xFC);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x05), ==, (uint8)0xFB);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x06), ==, (uint8)0xFA);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x07), ==, (uint8)0xF9);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x08), ==, (uint8)0xF8);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x09), ==, (uint8)0xF7);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x0A), ==, (uint8)0xF6);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x0B), ==, (uint8)0xF5);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x0C), ==, (uint8)0xF4);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x0D), ==, (uint8)0xF3);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x0E), ==, (uint8)0xF2);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x0F), ==, (uint8)0xF1);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x10), ==, (uint8)0xF0);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x11), ==, (uint8)0xEF);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x12), ==, (uint8)0xEE);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x13), ==, (uint8)0xED);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x14), ==, (uint8)0xEC);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x15), ==, (uint8)0xEB);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x16), ==, (uint8)0xEA);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x17), ==, (uint8)0xE9);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x18), ==, (uint8)0xE8);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x19), ==, (uint8)0xE7);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x1A), ==, (uint8)0xE6);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x1B), ==, (uint8)0xE5);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x1C), ==, (uint8)0xE4);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x1D), ==, (uint8)0xE3);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x1E), ==, (uint8)0xE2);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x1F), ==, (uint8)0xE1);
			TEST_ENSURE_OP(signedToTwosComplement((int8)-0x20), ==, (uint8)0xE0);

			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x00), ==, (uint8)0x00);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x01), ==, (uint8)0x01);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x02), ==, (uint8)0x02);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x03), ==, (uint8)0x03);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x04), ==, (uint8)0x04);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x05), ==, (uint8)0x05);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x06), ==, (uint8)0x06);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x07), ==, (uint8)0x07);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x08), ==, (uint8)0x08);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x09), ==, (uint8)0x09);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x0A), ==, (uint8)0x0A);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x0B), ==, (uint8)0x0B);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x0C), ==, (uint8)0x0C);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x0D), ==, (uint8)0x0D);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x0E), ==, (uint8)0x0E);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x0F), ==, (uint8)0x0F);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x10), ==, (uint8)0x10);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x11), ==, (uint8)0x11);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x12), ==, (uint8)0x12);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x13), ==, (uint8)0x13);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x14), ==, (uint8)0x14);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x15), ==, (uint8)0x15);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x16), ==, (uint8)0x16);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x17), ==, (uint8)0x17);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x18), ==, (uint8)0x18);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x19), ==, (uint8)0x19);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x1A), ==, (uint8)0x1A);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x1B), ==, (uint8)0x1B);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x1C), ==, (uint8)0x1C);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x1D), ==, (uint8)0x1D);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x1E), ==, (uint8)0x1E);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x1F), ==, (uint8)0x1F);
			TEST_ENSURE_OP(signedToTwosComplement((int8)+0x20), ==, (uint8)0x20);
		}

		void testTwosComplementToSigned()
		{
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x00), ==, (int8)+0x00);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xFF), ==, (int8)-0x01);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xFE), ==, (int8)-0x02);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xFD), ==, (int8)-0x03);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xFC), ==, (int8)-0x04);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xFB), ==, (int8)-0x05);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xFA), ==, (int8)-0x06);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xF9), ==, (int8)-0x07);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xF8), ==, (int8)-0x08);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xF7), ==, (int8)-0x09);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xF6), ==, (int8)-0x0A);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xF5), ==, (int8)-0x0B);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xF4), ==, (int8)-0x0C);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xF3), ==, (int8)-0x0D);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xF2), ==, (int8)-0x0E);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xF1), ==, (int8)-0x0F);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xF0), ==, (int8)-0x10);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xEF), ==, (int8)-0x11);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xEE), ==, (int8)-0x12);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xED), ==, (int8)-0x13);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xEC), ==, (int8)-0x14);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xEB), ==, (int8)-0x15);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xEA), ==, (int8)-0x16);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xE9), ==, (int8)-0x17);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xE8), ==, (int8)-0x18);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xE7), ==, (int8)-0x19);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xE6), ==, (int8)-0x1A);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xE5), ==, (int8)-0x1B);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xE4), ==, (int8)-0x1C);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xE3), ==, (int8)-0x1D);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xE2), ==, (int8)-0x1E);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xE1), ==, (int8)-0x1F);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0xE0), ==, (int8)-0x20);

			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x00), ==, (int8)+0x00);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x01), ==, (int8)+0x01);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x02), ==, (int8)+0x02);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x03), ==, (int8)+0x03);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x04), ==, (int8)+0x04);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x05), ==, (int8)+0x05);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x06), ==, (int8)+0x06);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x07), ==, (int8)+0x07);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x08), ==, (int8)+0x08);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x09), ==, (int8)+0x09);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x0A), ==, (int8)+0x0A);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x0B), ==, (int8)+0x0B);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x0C), ==, (int8)+0x0C);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x0D), ==, (int8)+0x0D);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x0E), ==, (int8)+0x0E);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x0F), ==, (int8)+0x0F);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x10), ==, (int8)+0x10);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x11), ==, (int8)+0x11);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x12), ==, (int8)+0x12);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x13), ==, (int8)+0x13);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x14), ==, (int8)+0x14);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x15), ==, (int8)+0x15);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x16), ==, (int8)+0x16);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x17), ==, (int8)+0x17);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x18), ==, (int8)+0x18);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x19), ==, (int8)+0x19);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x1A), ==, (int8)+0x1A);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x1B), ==, (int8)+0x1B);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x1C), ==, (int8)+0x1C);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x1D), ==, (int8)+0x1D);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x1E), ==, (int8)+0x1E);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x1F), ==, (int8)+0x1F);
			TEST_ENSURE_OP(twosComplementToSigned((uint8)0x20), ==, (int8)+0x20);
		}

		void testInverse()
		{
			for (uint8 i = 0;i < 255;++i)
			{
				int8 a = twosComplementToSigned(i);
				uint8 j = signedToTwosComplement(a);
				TEST_ENSURE_OP(i, ==, j);
			}

			int8 a = twosComplementToSigned((uint8)255);
			uint8 j = signedToTwosComplement(a);
			TEST_ENSURE_OP((uint8)255, ==, j);
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
				TEST_ENSURE_OP(i, == , j);
			}

			uint8 a = signedToTwosComplement((int8)127);
			int8 j = twosComplementToSigned(a);
			TEST_ENSURE_OP((int8)127, == , j);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("twos_complement", test);
	}

	CallFunction run(addTest);

}
