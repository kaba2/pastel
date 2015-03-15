// Description: Testing for locators
// DocumentationOf: locators.h

#include "test_pastelsys.h"

#include <pastel/sys/locator.h>

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
			testArray();
			testTypes();
		}

		void test()
		{
			integer d = 3;
			Pointer_Locator<real, 3> locator(d);
			real dataSet[] = {0, 1, 2, 3, 4, 5, 6};
			integer n = sizeof(dataSet) / sizeof(real);
			
			for (integer i = 0;i < n - d;++i)
			{
				for (integer j = 0;j < d;++j)
				{
					TEST_ENSURE(locator(dataSet + i, j) == i + j);
				}
			}
		}

		void testArray()
		{
			{
				using Locator = Array_Locator<real, 1>;
				PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);
			}
			{
				using Point = std::array<real, 2>;
				using Locator = Array_Locator<real, 2>;
				PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);

				PASTEL_STATIC_ASSERT(
					(std::is_same<Locator_Real<Locator>, real>::value));

				PASTEL_STATIC_ASSERT(
					(std::is_same<Locator_Point<Locator>, Point>::value));
				
				PASTEL_STATIC_ASSERT(
					Locator_N<Locator>::value == 2);
			}
		}

		void testTypes()
		{
			PASTEL_STATIC_ASSERT(
				(std::is_same<Locator_Real<Pointer_Locator<real>>, real>::value));

			PASTEL_STATIC_ASSERT(
				(std::is_same<Locator_Real<Pointer_Locator<real>, Pointer_Locator<integer>>, real>::value));

			PASTEL_STATIC_ASSERT(
				(std::is_same<Locator_Real<Pointer_Locator<integer>, Pointer_Locator<real>>, real>::value));

			PASTEL_STATIC_ASSERT(
				(std::is_same<Locator_Point<Pointer_Locator<integer>>, const integer*>::value));

			TEST_ENSURE_OP((Locator_N<Pointer_Locator<real, 0>>::value), ==, 0);
			TEST_ENSURE_OP((Locator_N<Pointer_Locator<real, 1>>::value), ==, 1);
			TEST_ENSURE_OP((Locator_N<Pointer_Locator<real, 2>>::value), ==, 2);
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
