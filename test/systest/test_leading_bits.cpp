// Description: Testing for leading zero/one bits
// DocumentationOf: leading_zero_bits.h

#include "test_pastelsys.h"

#include <pastel/sys/bit/leading_zero_bits.h>
#include <pastel/sys/bit/leading_one_bits.h>

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
			testZeros();
			testOnes();
		}

		void testZeros()
		{
			auto zeros = [](uint8 that, integer correct)
			{
				return leadingZeroBits(that) == correct;
			};

			TEST_ENSURE(zeros(0x00, 8));
			TEST_ENSURE(zeros(0x01, 0));
			TEST_ENSURE(zeros(0x02, 1));
			TEST_ENSURE(zeros(0x03, 0));
			TEST_ENSURE(zeros(0x04, 2));
			TEST_ENSURE(zeros(0x05, 0));
			TEST_ENSURE(zeros(0x06, 1));
			TEST_ENSURE(zeros(0x07, 0));
			TEST_ENSURE(zeros(0x08, 3));
			TEST_ENSURE(zeros(0x09, 0));
			TEST_ENSURE(zeros(0x0A, 1));
			TEST_ENSURE(zeros(0x0B, 0));
			TEST_ENSURE(zeros(0x0C, 2));
			TEST_ENSURE(zeros(0x0D, 0));
			TEST_ENSURE(zeros(0x0E, 1));
			TEST_ENSURE(zeros(0x0F, 0));
			TEST_ENSURE(zeros(0x10, 4 + 0));
			TEST_ENSURE(zeros(0x20, 4 + 1));
			TEST_ENSURE(zeros(0x30, 4 + 0));
			TEST_ENSURE(zeros(0x40, 4 + 2));
			TEST_ENSURE(zeros(0x50, 4 + 0));
			TEST_ENSURE(zeros(0x60, 4 + 1));
			TEST_ENSURE(zeros(0x70, 4 + 0));
			TEST_ENSURE(zeros(0x80, 4 + 3));
			TEST_ENSURE(zeros(0x90, 4 + 0));
			TEST_ENSURE(zeros(0xA0, 4 + 1));
			TEST_ENSURE(zeros(0xB0, 4 + 0));
			TEST_ENSURE(zeros(0xC0, 4 + 2));
			TEST_ENSURE(zeros(0xD0, 4 + 0));
			TEST_ENSURE(zeros(0xE0, 4 + 1));
			TEST_ENSURE(zeros(0xF0, 4 + 0));
		}

		void testOnes()
		{
			auto ones = [](uint8 that, integer correct)
			{
				return leadingOneBits(that) == correct;
			};

			TEST_ENSURE(ones(0x00, 0));
			TEST_ENSURE(ones(0x01, 1));
			TEST_ENSURE(ones(0x02, 0));
			TEST_ENSURE(ones(0x03, 2));
			TEST_ENSURE(ones(0x04, 0));
			TEST_ENSURE(ones(0x05, 1));
			TEST_ENSURE(ones(0x06, 0));
			TEST_ENSURE(ones(0x07, 3));
			TEST_ENSURE(ones(0x08, 0));
			TEST_ENSURE(ones(0x09, 1));
			TEST_ENSURE(ones(0x0A, 0));
			TEST_ENSURE(ones(0x0B, 2));
			TEST_ENSURE(ones(0x0C, 0));
			TEST_ENSURE(ones(0x0D, 1));
			TEST_ENSURE(ones(0x0E, 0));
			TEST_ENSURE(ones(0x0F, 4));
			TEST_ENSURE(ones(0x1F, 4 + 1));
			TEST_ENSURE(ones(0x2F, 4 + 0));
			TEST_ENSURE(ones(0x3F, 4 + 2));
			TEST_ENSURE(ones(0x4F, 4 + 0));
			TEST_ENSURE(ones(0x5F, 4 + 1));
			TEST_ENSURE(ones(0x6F, 4 + 0));
			TEST_ENSURE(ones(0x7F, 4 + 3));
			TEST_ENSURE(ones(0x8F, 4 + 0));
			TEST_ENSURE(ones(0x9F, 4 + 1));
			TEST_ENSURE(ones(0xAF, 4 + 0));
			TEST_ENSURE(ones(0xBF, 4 + 2));
			TEST_ENSURE(ones(0xCF, 4 + 0));
			TEST_ENSURE(ones(0xDF, 4 + 1));
			TEST_ENSURE(ones(0xEF, 4 + 0));
			TEST_ENSURE(ones(0xFF, 4 + 4));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("leadingZeroBits", test);
	}

	CallFunction run(addTest);

}
