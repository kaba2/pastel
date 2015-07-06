// Description: Testing for intervals
// DocumentationOf: interval.h

#include "test_pastelsys.h"

#include <pastel/sys/interval.h>

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
			using I = Interval<real>;

			I a(1, 4);
			{
				TEST_ENSURE(a.min() == 1);
				TEST_ENSURE(a.max() == 4);
			}
			{
				a.min() = 3;
				a.max() = 5;

				TEST_ENSURE(a.min() == 3);
				TEST_ENSURE(a.max() == 5);
			}
			{
				a.set(2, 6);
				TEST_ENSURE(a.min() == 2);
				TEST_ENSURE(a.max() == 6);
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
		testRunner().add("interval", test);
	}

	CallFunction run(addTest);

}
