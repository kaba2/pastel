// Description: Testing for locators
// DocumentationOf: locators.h

#include "test_pastelsys.h"

#include <pastel/sys/locators.h>

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
			test();
		}

		void test()
		{
			const integer d = 3;
			Pointer_Locator<real, 3> locator(d);
			real dataSet[] = {0, 1, 2, 3, 4, 5, 6};
			const integer n = sizeof(dataSet) / sizeof(real);
			
			for (integer i = 0;i < n - d;++i)
			{
				for (integer j = 0;j < d;++j)
				{
					TEST_ENSURE(locator(dataSet + i, j) == i + j);
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
		testRunner().add("Locator", test);
	}

	CallFunction run(addTest);

}
