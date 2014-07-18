// Description: Testing for bit masks
// DocumentationOf: bitmask.h

#include "test_pastelsys.h"
#include <pastel/sys/bitmask.h>

using namespace Pastel;

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
			test();
		}

		void test()
		{
			TEST_ENSURE_OP(singleBitMask<uint8>(0), ==, 1);
			TEST_ENSURE_OP(singleBitMask<uint8>(1), ==, 2);
			TEST_ENSURE_OP(singleBitMask<uint8>(2), ==, 4);
			TEST_ENSURE_OP(singleBitMask<uint8>(3), ==, 8);
			TEST_ENSURE_OP(singleBitMask<uint8>(4), ==, 16);
			TEST_ENSURE_OP(singleBitMask<uint8>(5), ==, 32);
			TEST_ENSURE_OP(singleBitMask<uint8>(6), ==, 64);
			TEST_ENSURE_OP(singleBitMask<uint8>(7), ==, 128);

			TEST_ENSURE_OP(bitMask<uint8>(0), ==, 0x00);
			TEST_ENSURE_OP(bitMask<uint8>(1), ==, 0x01);
			TEST_ENSURE_OP(bitMask<uint8>(2), ==, 0x03);
			TEST_ENSURE_OP(bitMask<uint8>(3), ==, 0x07);
			TEST_ENSURE_OP(bitMask<uint8>(4), ==, 0x0F);
			TEST_ENSURE_OP(bitMask<uint8>(5), ==, 0x1F);
			TEST_ENSURE_OP(bitMask<uint8>(6), ==, 0x3F);
			TEST_ENSURE_OP(bitMask<uint8>(7), ==, 0x7F);
			TEST_ENSURE_OP(bitMask<uint8>(8), ==, 0xFF);

			TEST_ENSURE_OP(bitMask<uint8>(0, 0), ==, 0x00);
			TEST_ENSURE_OP(bitMask<uint8>(0, 1), ==, 0x01);
			TEST_ENSURE_OP(bitMask<uint8>(0, 2), ==, 0x03);
			TEST_ENSURE_OP(bitMask<uint8>(0, 3), ==, 0x07);
			TEST_ENSURE_OP(bitMask<uint8>(0, 4), ==, 0x0F);
			TEST_ENSURE_OP(bitMask<uint8>(0, 5), ==, 0x1F);
			TEST_ENSURE_OP(bitMask<uint8>(0, 6), ==, 0x3F);
			TEST_ENSURE_OP(bitMask<uint8>(0, 7), ==, 0x7F);
			TEST_ENSURE_OP(bitMask<uint8>(0, 8), ==, 0xFF);

			TEST_ENSURE_OP(bitMask<uint8>(1, 8), ==, 0xFE);
			TEST_ENSURE_OP(bitMask<uint8>(2, 8), ==, 0xFC);
			TEST_ENSURE_OP(bitMask<uint8>(3, 8), ==, 0xF8);
			TEST_ENSURE_OP(bitMask<uint8>(4, 8), ==, 0xF0);
			TEST_ENSURE_OP(bitMask<uint8>(5, 8), ==, 0xE0);
			TEST_ENSURE_OP(bitMask<uint8>(6, 8), ==, 0xC0);
			TEST_ENSURE_OP(bitMask<uint8>(7, 8), ==, 0x80);
			TEST_ENSURE_OP(bitMask<uint8>(8, 8), ==, 0x00);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("bitMask", test);
	}

	CallFunction run(addTest);

}
