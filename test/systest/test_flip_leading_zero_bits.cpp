// Description: Testing for flipping leading zero bits
// DocumentationOf: flip_leading_zero_bits.h

#include "test_pastelsys.h"
#include <pastel/sys/flip_leading_zero_bits.h>

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
			test();
		}

		void test()
		{
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000000), ==, 0xFFFFFFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000001), ==, 0x00000001);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000002), ==, 0x00000003);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000004), ==, 0x00000007);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000008), ==, 0x0000000F);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000010), ==, 0x0000001F);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000020), ==, 0x0000003F);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000040), ==, 0x0000007F);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000080), ==, 0x000000FF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000100), ==, 0x000001FF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000200), ==, 0x000003FF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000400), ==, 0x000007FF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000800), ==, 0x00000FFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00001000), ==, 0x00001FFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00002000), ==, 0x00003FFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00004000), ==, 0x00007FFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00008000), ==, 0x0000FFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00010000), ==, 0x0001FFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00020000), ==, 0x0003FFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00040000), ==, 0x0007FFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00080000), ==, 0x000FFFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00100000), ==, 0x001FFFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00200000), ==, 0x003FFFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00400000), ==, 0x007FFFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00800000), ==, 0x00FFFFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x01000000), ==, 0x01FFFFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x02000000), ==, 0x03FFFFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x04000000), ==, 0x07FFFFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x08000000), ==, 0x0FFFFFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x10000000), ==, 0x1FFFFFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x20000000), ==, 0x3FFFFFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x40000000), ==, 0x7FFFFFFF);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x80000000), ==, 0xFFFFFFFF);

			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654321), ==, 0x87654321);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654322), ==, 0x87654323);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654323), ==, 0x87654323);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654324), ==, 0x87654327);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654325), ==, 0x87654325);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654326), ==, 0x87654327);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654327), ==, 0x87654327);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654328), ==, 0x8765432F);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654329), ==, 0x87654329);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x8765432A), ==, 0x8765432B);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x8765432B), ==, 0x8765432B);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x8765432C), ==, 0x8765432F);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x8765432D), ==, 0x8765432D);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x8765432E), ==, 0x8765432F);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x8765432F), ==, 0x8765432F);

			TEST_ENSURE_OP(flipLeadingZeroBits(0x3501), ==, 0x3501);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x3510), ==, 0x351F);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x3100), ==, 0x31FF);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("flipLeadingZeroBits", test);
	}

	CallFunction run(addTest);

}
