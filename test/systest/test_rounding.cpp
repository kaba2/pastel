// Description: Testing for rounding
// DocumentationOf: rounding.h

#include "test_pastelsys.h"

#include "pastel/sys/rounding.h"

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
			TEST_ENSURE_OP(roundUpToPowerOf2(0), == , 1);
			TEST_ENSURE_OP(roundUpToPowerOf2(1), == , 1);
			TEST_ENSURE_OP(roundUpToPowerOf2(2), == , 2);
			TEST_ENSURE_OP(roundUpToPowerOf2(3), == , 4);
			TEST_ENSURE_OP(roundUpToPowerOf2(5), == , 8);
			TEST_ENSURE_OP(roundUpToPowerOf2(6), == , 8);
			TEST_ENSURE_OP(roundUpToPowerOf2(8), == , 8);
			TEST_ENSURE_OP(roundUpToPowerOf2(9), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOf2(9), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOf2(10), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOf2(11), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOf2(12), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOf2(13), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOf2(14), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOf2(15), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOf2(16), == , 16);
			TEST_ENSURE_OP(roundUpToPowerOf2(0x0000000F), == , 0x00000010);
			TEST_ENSURE_OP(roundUpToPowerOf2(0x000000FF), == , 0x00000100);
			TEST_ENSURE_OP(roundUpToPowerOf2(0x00000FFF), == , 0x00001000);
			TEST_ENSURE_OP(roundUpToPowerOf2(0x00000FFF), == , 0x00001000);
			TEST_ENSURE_OP(roundUpToPowerOf2(0x0000FFFF), == , 0x00010000);
			TEST_ENSURE_OP(roundUpToPowerOf2(0x0FFFFFFF), == , 0x10000000);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTests()
	{
		testRunner().add("rounding", test);
	}

	CallFunction run(addTests);

}
