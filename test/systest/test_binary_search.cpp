// Description: Testing for binary search
// DocumentationOf: binary_search.h

#include "test_pastelsys.h"

#include <pastel/sys/sequence/binary_search.h>
#include <pastel/sys/sequence/exponential_binary_search.h>
#include <pastel/sys/indicator/predicate_indicator.h>

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
			testInteger();
		}

		void testInteger()
		{
			{
				for (integer b = 0;b <= 256;++b)
				{
					integer a = binarySearch(
						0, 256, [&](integer n) {return n >= b;});

					TEST_ENSURE_OP(a, ==, b);
				}
			}

			{
				for (integer b = 0;b <= 256;++b)
				{
					integer a = exponentialBinarySearch(
						0, 256, [&](integer n) {return n >= b;});

					TEST_ENSURE_OP(a, ==, b);
				}
			}
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("binarySearch", test);
	}

	CallFunction run(addTest);

}
