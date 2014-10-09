// Description: Testing for native integers
// DocumentationOf: native_integer.h

#include "test_pastelsys.h"
#include <pastel/sys/number_tests.h>

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
			testInfinity();
			testEvenOdd();
			testIsPowerOfTwo();
		}

		void testInfinity()
		{
			TEST_ENSURE(infinity<uint8>() == 0xFFu);
			TEST_ENSURE(infinity<uint16>() == 0xFFFFu);
			TEST_ENSURE(infinity<uint32>() == 0xFFFFFFFFul);
			TEST_ENSURE(infinity<uint64>() == 0xFFFFFFFFFFFFFFFFull);

			TEST_ENSURE(infinity<int8>() == 0x7Fu);
			TEST_ENSURE(infinity<int16>() == 0x7FFFu);
			TEST_ENSURE(infinity<int32>() == 0x7FFFFFFFul);
			TEST_ENSURE(infinity<int64>() == 0x7FFFFFFFFFFFFFFFull);
		}

		void testEvenOdd()
		{
			for (integer i = 1;i < 100;i += 2)
			{
				TEST_ENSURE(odd((char)i));
				TEST_ENSURE(odd((short)i));
				TEST_ENSURE(odd((int)i));
				TEST_ENSURE(odd((long)i));
				TEST_ENSURE(odd((longlong)i));

				TEST_ENSURE(odd((uchar)i));
				TEST_ENSURE(odd((ushort)i));
				TEST_ENSURE(odd((uint)i));
				TEST_ENSURE(odd((ulong)i));
				TEST_ENSURE(odd((ulonglong)i));
			}

			for (integer i = 0;i < 100;i += 2)
			{
				TEST_ENSURE(even((char)i));
				TEST_ENSURE(even((short)i));
				TEST_ENSURE(even((int)i));
				TEST_ENSURE(even((long)i));
				TEST_ENSURE(even((longlong)i));
				
				TEST_ENSURE(even((uchar)i));
				TEST_ENSURE(even((ushort)i));
				TEST_ENSURE(even((uint)i));
				TEST_ENSURE(even((ulong)i));
				TEST_ENSURE(even((ulonglong)i));
			}
		}

		void testIsPowerOfTwo()
		{
			TEST_ENSURE(isPowerOfTwo(1));
			TEST_ENSURE(isPowerOfTwo(2));
			TEST_ENSURE(!isPowerOfTwo(3));
			TEST_ENSURE(isPowerOfTwo(4));
			TEST_ENSURE(!isPowerOfTwo(5));
			TEST_ENSURE(!isPowerOfTwo(6));
			TEST_ENSURE(!isPowerOfTwo(7));
			TEST_ENSURE(isPowerOfTwo(8));
			TEST_ENSURE(!isPowerOfTwo(9));
			TEST_ENSURE(!isPowerOfTwo(10));
			TEST_ENSURE(!isPowerOfTwo(11));
			TEST_ENSURE(!isPowerOfTwo(12));
			TEST_ENSURE(!isPowerOfTwo(13));
			TEST_ENSURE(!isPowerOfTwo(14));
			TEST_ENSURE(!isPowerOfTwo(15));
			TEST_ENSURE(isPowerOfTwo(16));
			TEST_ENSURE(!isPowerOfTwo(17));
			TEST_ENSURE(!isPowerOfTwo(18));
			TEST_ENSURE(!isPowerOfTwo(19));
			TEST_ENSURE(!isPowerOfTwo(20));
			TEST_ENSURE(!isPowerOfTwo(21));
			TEST_ENSURE(!isPowerOfTwo(22));
			TEST_ENSURE(!isPowerOfTwo(23));
			TEST_ENSURE(!isPowerOfTwo(24));
			TEST_ENSURE(!isPowerOfTwo(25));
			TEST_ENSURE(!isPowerOfTwo(26));
			TEST_ENSURE(!isPowerOfTwo(27));
			TEST_ENSURE(!isPowerOfTwo(28));
			TEST_ENSURE(!isPowerOfTwo(29));
			TEST_ENSURE(!isPowerOfTwo(30));
			TEST_ENSURE(!isPowerOfTwo(31));
			TEST_ENSURE(isPowerOfTwo(32));
			TEST_ENSURE(isPowerOfTwo(64));
			TEST_ENSURE(isPowerOfTwo(128));
			TEST_ENSURE(isPowerOfTwo(256));
			TEST_ENSURE(isPowerOfTwo(512));
			TEST_ENSURE(!isPowerOfTwo(511));
			TEST_ENSURE(!isPowerOfTwo(513));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("native_integer", test);
	}

	CallFunction run(addTest);

}
