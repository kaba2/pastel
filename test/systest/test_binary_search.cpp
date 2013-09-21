// Description: Testing for binary search
// DocumentationOf: binary_search.h

#include "test_pastelsys.h"

#include <pastel/sys/binary_search.h>
#include <pastel/sys/predicate_indicator.h>

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
			testIterator();
			testInteger();
		}

		void testInteger()
		{
			{
				for (integer b = 0;b < 7;++b)
				{
					integer a = binarySearch(
						0, 7,
						predicateIndicator(b, LessThan()));

					TEST_ENSURE_OP(a, ==, b);
				}
			}
			{
					integer a = binarySearch(
						0, 7,
						predicateIndicator(7, LessThan()));

					TEST_ENSURE_OP(a, ==, 7);
			}
		}

		void testIterator()
		{
			integer aSet[] = {0, 1, 2, 3, 4, 5, 6};
			{
				for (integer* b = aSet;b != std::end(aSet);++b)
				{
					integer* a = binarySearch(
						std::begin(aSet), std::end(aSet),
						[&](integer* level)
					{
						return *level < *b;
					});

					TEST_ENSURE_OP(a - aSet, ==, b - aSet);
				}
			}
			{
				integer* a = binarySearch(
					std::begin(aSet), std::end(aSet),
					[&](integer* level)
				{
					return *level < 7;
				});

				TEST_ENSURE_OP(a - aSet, ==, 7);
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
