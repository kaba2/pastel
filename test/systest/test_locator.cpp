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
			testTypes();
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

		void testTypes()
		{
			TEST_ENSURE(
				(std::is_same<Locator_Real<Pointer_Locator<real>>, real>()));

			TEST_ENSURE(
				(std::is_same<Locator_Real<Pointer_Locator<real>, Pointer_Locator<integer>>, real>()));

			TEST_ENSURE(
				(std::is_same<Locator_Real<Pointer_Locator<integer>, Pointer_Locator<real>>, real>()));

			TEST_ENSURE_OP((Locator_N<Pointer_Locator<real, 0>>()), ==, 0);
			TEST_ENSURE_OP((Locator_N<Pointer_Locator<real, 1>>()), ==, 1);
			TEST_ENSURE_OP((Locator_N<Pointer_Locator<real, 2>>()), ==, 2);
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
