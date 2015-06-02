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
			testBinarySearch<int8>();
			testBinarySearch<int16>();
			testBinarySearch<int32>();
			testBinarySearch<integer>();

			testBinarySearch<uint8>();
			testBinarySearch<uint16>();
			testBinarySearch<uint32>();
			testBinarySearch<uinteger>();
		}

		template <typename Integer>
		void testBinarySearch()
		{
			Integer min = std::is_signed<Integer>::value ? -128 : 0;
			Integer max = std::is_signed<Integer>::value ? 127 : 255;

			{
				// This is deliberately integer instead of Integer, 
				// because otherwise uint8 would loop forever.
				for (Integer b = min;b < max;++b)
				{
					Integer a = binarySearch(
						(Integer)min, (Integer)max, [&](Integer n) {return n >= b;});

					TEST_ENSURE_OP(a, ==, (Integer)b);
				}
			}

			{
				for (Integer b = min;b < max;++b)
				{
					Integer a = exponentialBinarySearch(
						(Integer)min, (Integer)max, [&](Integer n) {return n >= b;});

					TEST_ENSURE_OP(a, ==, (Integer)b);
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
