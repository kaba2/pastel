// Description: Testing for flipping leading bits
// DocumentationOf: flip_leading_zero_bits.h

#include "test_pastelsys.h"

#include <pastel/sys/flip_leading_zero_bits.h>
#include <pastel/sys/flip_leading_one_bits.h>

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
			testZero();
			testOne();
		}

		void testZero()
		{
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000000u), ==, 0xFFFFFFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000001u), ==, 0x00000001u);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000002u), ==, 0x00000003u);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000004u), ==, 0x00000007u);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000008u), ==, 0x0000000Fu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000010u), ==, 0x0000001Fu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000020u), ==, 0x0000003Fu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000040u), ==, 0x0000007Fu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000080u), ==, 0x000000FFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000100u), ==, 0x000001FFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000200u), ==, 0x000003FFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000400u), ==, 0x000007FFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00000800u), ==, 0x00000FFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00001000u), ==, 0x00001FFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00002000u), ==, 0x00003FFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00004000u), ==, 0x00007FFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00008000u), ==, 0x0000FFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00010000u), ==, 0x0001FFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00020000u), ==, 0x0003FFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00040000u), ==, 0x0007FFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00080000u), ==, 0x000FFFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00100000u), ==, 0x001FFFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00200000u), ==, 0x003FFFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00400000u), ==, 0x007FFFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x00800000u), ==, 0x00FFFFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x01000000u), ==, 0x01FFFFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x02000000u), ==, 0x03FFFFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x04000000u), ==, 0x07FFFFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x08000000u), ==, 0x0FFFFFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x10000000u), ==, 0x1FFFFFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x20000000u), ==, 0x3FFFFFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x40000000u), ==, 0x7FFFFFFFu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x80000000u), ==, 0xFFFFFFFFu);

			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654321u), ==, 0x87654321u);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654322u), ==, 0x87654323u);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654323u), ==, 0x87654323u);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654324u), ==, 0x87654327u);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654325u), ==, 0x87654325u);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654326u), ==, 0x87654327u);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654327u), ==, 0x87654327u);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654328u), ==, 0x8765432Fu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x87654329u), ==, 0x87654329u);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x8765432Au), ==, 0x8765432Bu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x8765432Bu), ==, 0x8765432Bu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x8765432Cu), ==, 0x8765432Fu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x8765432Du), ==, 0x8765432Du);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x8765432Eu), ==, 0x8765432Fu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x8765432Fu), ==, 0x8765432Fu);

			TEST_ENSURE_OP(flipLeadingZeroBits(0x3501u), ==, 0x3501u);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x3510u), ==, 0x351Fu);
			TEST_ENSURE_OP(flipLeadingZeroBits(0x3100u), ==, 0x31FFu);
		}

		void testOne()
		{
			TEST_ENSURE_OP(flipLeadingOneBits(0x86420000u), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x86420001u), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x86420003u), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x86420007u), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x8642000Fu), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x8642001Fu), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x8642003Fu), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x8642007Fu), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x864200FFu), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x864201FFu), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x864203FFu), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x864207FFu), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x86420FFFu), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x86421FFFu), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x86423FFFu), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x86427FFFu), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x8642FFFFu), ==, 0x86420000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x8641FFFFu), ==, 0x86400000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x8643FFFFu), ==, 0x86400000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x8647FFFFu), ==, 0x86400000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x864FFFFFu), ==, 0x86400000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x861FFFFFu), ==, 0x86000000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x863FFFFFu), ==, 0x86000000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x867FFFFFu), ==, 0x86000000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x86FFFFFFu), ==, 0x86000000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x81FFFFFFu), ==, 0x80000000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x83FFFFFFu), ==, 0x80000000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x87FFFFFFu), ==, 0x80000000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x8FFFFFFFu), ==, 0x80000000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x1FFFFFFFu), ==, 0x00000000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x3FFFFFFFu), ==, 0x00000000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x7FFFFFFFu), ==, 0x00000000u);
			TEST_ENSURE_OP(flipLeadingOneBits(0xFFFFFFFFu), ==, 0x00000000u);

			TEST_ENSURE_OP(flipLeadingOneBits(0x87654321u), ==, 0x87654320u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x87654322u), ==, 0x87654322u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x87654323u), ==, 0x87654320u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x87654324u), ==, 0x87654324u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x87654325u), ==, 0x87654324u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x87654326u), ==, 0x87654326u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x87654327u), ==, 0x87654320u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x87654328u), ==, 0x87654328u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x87654329u), ==, 0x87654328u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x8765432Au), ==, 0x8765432Au);
			TEST_ENSURE_OP(flipLeadingOneBits(0x8765432Bu), ==, 0x87654328u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x8765432Cu), ==, 0x8765432Cu);
			TEST_ENSURE_OP(flipLeadingOneBits(0x8765432Du), ==, 0x8765432Cu);
			TEST_ENSURE_OP(flipLeadingOneBits(0x8765432Eu), ==, 0x8765432Eu);
			TEST_ENSURE_OP(flipLeadingOneBits(0x8765432Fu), ==, 0x87654320u);

			TEST_ENSURE_OP(flipLeadingOneBits(0x3501u), ==, 0x3500u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x3510u), ==, 0x3510u);
			TEST_ENSURE_OP(flipLeadingOneBits(0x3100u), ==, 0x3100u);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("flipLeadingBits", test);
	}

	CallFunction run(addTest);

}
