// Description: Testing for zeroing higher bits
// DocumentationOf: zero_higher_bits.h

#include "test_pastelsys.h"
#include <pastel/sys/zero_higher_bits.h>

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
			TEST_ENSURE_OP(zeroHigherBits(0x00000000), ==, 0x00000000);
			TEST_ENSURE_OP(zeroHigherBits(0x87654321), ==, 0x00000001);
			TEST_ENSURE_OP(zeroHigherBits(0x87654322), ==, 0x00000002);
			TEST_ENSURE_OP(zeroHigherBits(0x87654323), ==, 0x00000001);
			TEST_ENSURE_OP(zeroHigherBits(0x87654324), ==, 0x00000004);
			TEST_ENSURE_OP(zeroHigherBits(0x87654325), ==, 0x00000001);
			TEST_ENSURE_OP(zeroHigherBits(0x87654326), ==, 0x00000002);
			TEST_ENSURE_OP(zeroHigherBits(0x87654327), ==, 0x00000001);
			TEST_ENSURE_OP(zeroHigherBits(0x87654328), ==, 0x00000008);
			TEST_ENSURE_OP(zeroHigherBits(0x87654329), ==, 0x00000001);
			TEST_ENSURE_OP(zeroHigherBits(0x8765432A), ==, 0x00000002);
			TEST_ENSURE_OP(zeroHigherBits(0x8765432B), ==, 0x00000001);
			TEST_ENSURE_OP(zeroHigherBits(0x8765432C), ==, 0x00000004);
			TEST_ENSURE_OP(zeroHigherBits(0x8765432D), ==, 0x00000001);
			TEST_ENSURE_OP(zeroHigherBits(0x8765432E), ==, 0x00000002);
			TEST_ENSURE_OP(zeroHigherBits(0x8765432F), ==, 0x00000001);

			TEST_ENSURE_OP(zeroHigherBits(0x0010), ==, 0x0010);
			TEST_ENSURE_OP(zeroHigherBits(0x0100), ==, 0x0100);
			TEST_ENSURE_OP(zeroHigherBits(0x1000), ==, 0x1000);

			TEST_ENSURE_OP(zeroHigherBits(0x3501), ==, 0x0001);
			TEST_ENSURE_OP(zeroHigherBits(0x3510), ==, 0x0010);
			TEST_ENSURE_OP(zeroHigherBits(0x3100), ==, 0x0100);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("zeroHigherBits", test);
	}

	CallFunction run(addTest);

}
