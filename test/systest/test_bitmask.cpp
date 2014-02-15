// Description: Testing for bit masks
// DocumentationOf: bitmask.h

#include "test_pastelsys.h"
#include <pastel/sys/bitmask.h>

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
			TEST_ENSURE_OP(singleBitMask<uinteger>(0), ==, 1);
			TEST_ENSURE_OP(singleBitMask<uinteger>(1), ==, 2);
			TEST_ENSURE_OP(singleBitMask<uinteger>(2), ==, 4);
			TEST_ENSURE_OP(singleBitMask<uinteger>(3), ==, 8);
			TEST_ENSURE_OP(singleBitMask<uinteger>(4), ==, 16);
			TEST_ENSURE_OP(singleBitMask<uinteger>(5), ==, 32);

			TEST_ENSURE_OP(bitMask<uinteger>(0), ==, 0);
			TEST_ENSURE_OP(bitMask<uinteger>(1), ==, 1);
			TEST_ENSURE_OP(bitMask<uinteger>(2), ==, 3);
			TEST_ENSURE_OP(bitMask<uinteger>(3), ==, 7);
			TEST_ENSURE_OP(bitMask<uinteger>(4), ==, 15);
			TEST_ENSURE_OP(bitMask<uinteger>(5), ==, 31);

			TEST_ENSURE_OP(bitMask<uinteger>(0, 0), ==, 0);
			TEST_ENSURE_OP(bitMask<uinteger>(0, 1), ==, 1);
			TEST_ENSURE_OP(bitMask<uinteger>(0, 2), ==, 3);
			TEST_ENSURE_OP(bitMask<uinteger>(0, 3), ==, 7);
			TEST_ENSURE_OP(bitMask<uinteger>(0, 4), ==, 15);
			TEST_ENSURE_OP(bitMask<uinteger>(0, 5), ==, 31);
			TEST_ENSURE_OP(bitMask<uinteger>(1, 5), ==, 30);
			TEST_ENSURE_OP(bitMask<uinteger>(2, 5), ==, 28);
			TEST_ENSURE_OP(bitMask<uinteger>(3, 5), ==, 24);
			TEST_ENSURE_OP(bitMask<uinteger>(4, 5), ==, 16);
			TEST_ENSURE_OP(bitMask<uinteger>(5, 5), ==, 0);
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
