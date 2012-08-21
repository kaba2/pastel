// Description: Testing for minMax
// DocumentationOf: minmax.h

#include "test_pastelsys.h"

#include "pastel/sys/minmax.h"

namespace
{

	using namespace Pastel;

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
			testMinMax();
		}

		void testMinMax()
		{
			integer min = 0;
			integer max = 0;

			minMax(1, 2, 3, min, max);
			TEST_ENSURE(min == 1 && max == 3);

			minMax(1.0, 2, 3.0f, min, max);
			TEST_ENSURE(min == 1 && max == 3);

			minMax(1, 3, 2, min, max);
			TEST_ENSURE(min == 1 && max == 3);

			minMax(2, 1, 3, min, max);
			TEST_ENSURE(min == 1 && max == 3);

			minMax(2, 3, 1, min, max);
			TEST_ENSURE(min == 1 && max == 3);

			minMax(3, 1, 2, min, max);
			TEST_ENSURE(min == 1 && max == 3);

			minMax(3, 2, 1, min, max);
			TEST_ENSURE(min == 1 && max == 3);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("MinMax", test);
	}

	CallFunction run(addTest);

}
