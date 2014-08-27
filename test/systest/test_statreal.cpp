// Description: Testing for StatReal
// DocumentationOf: statreal.h

#include "test_pastelsys.h"

#include "pastel/sys/statreal.h"
#include "pastel/sys/vector.h"

#include <iostream>

using namespace Pastel;

namespace
{

	using namespace StatReal;

	class Test
		: public TestSuite
	{
	public:
		using Real = Number<real>;

		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			testConstruction();
			testOperations();
			testComparisons();
			testConversions();
		}

		void testConstruction()
		{
			globalStats().clear();

			TEST_ENSURE(allEqual(globalStats().statistics(), 0));
			TEST_ENSURE_OP(globalStats().constructed(), ==, 0);
			TEST_ENSURE_OP(globalStats().destructed(), ==, 0);

			Real a;

			TEST_ENSURE(allEqual(globalStats().statistics(), 0));
			TEST_ENSURE_OP(globalStats().constructed(), ==, 1);
			TEST_ENSURE_OP(globalStats().destructed(), ==, 0);
			
			{
				TEST_ENSURE(allEqual(globalStats().statistics(), 0));
				TEST_ENSURE_OP(globalStats().constructed(), ==, 1);
				TEST_ENSURE_OP(globalStats().destructed(), ==, 0);

				Real b;

				TEST_ENSURE(allEqual(globalStats().statistics(), 0));
				TEST_ENSURE_OP(globalStats().constructed(), ==, 2);
				TEST_ENSURE_OP(globalStats().destructed(), ==, 0);
			}

			{
				TEST_ENSURE(allEqual(globalStats().statistics(), 0));
				TEST_ENSURE_OP(globalStats().constructed(), ==, 2);
				TEST_ENSURE_OP(globalStats().destructed(), ==, 1);

				Real b(a);

				TEST_ENSURE(allEqual(globalStats().statistics(), 0));
				TEST_ENSURE_OP(globalStats().constructed(), ==, 3);
				TEST_ENSURE_OP(globalStats().destructed(), ==, 1);
			}

			{
				TEST_ENSURE(allEqual(globalStats().statistics(), 0));
				TEST_ENSURE_OP(globalStats().constructed(), ==, 3);
				TEST_ENSURE_OP(globalStats().destructed(), ==, 2);

				Real b(4);

				TEST_ENSURE(allEqual(globalStats().statistics(), 0));
				TEST_ENSURE_OP(globalStats().constructed(), ==, 4);
				TEST_ENSURE_OP(globalStats().destructed(), ==, 2);
			}

			TEST_ENSURE(allEqual(globalStats().statistics(), 0));
			TEST_ENSURE_OP(globalStats().constructed(), ==, 4);
			TEST_ENSURE_OP(globalStats().destructed(), ==, 3);

		}

		void testOperations()
		{
			globalStats().clear();
			{
				Real a;
				a += a;
				a -= a;
				a *= a;
				a /= a;
				a = a;

				a + a;
				a - a;
				a * a;
				a / a;
			}
			// Operation count 9, no other stats.
			TEST_ENSURE(allEqual(evaluate(globalStats().statistics() - 
				9 * unitAxis<integer, Stat::Enum::Size>(Stat::Enum::Operations)), 0));
		}

		void testComparisons()
		{
			globalStats().clear();

			{
				Real a;
				a < a;
				a <= a;
				a > a;
				a >= a;
				a == a;
				a != a;
			}
			// Comparison count 6, no other stats.
			TEST_ENSURE(allEqual(evaluate(globalStats().statistics() - 
				6 * unitAxis<integer, Stat::Enum::Size>(Stat::Enum::Comparisons)), 0));
		}

		real f(real x)
		{
			return x;
		}

		void testConversions()
		{
			globalStats().clear();

			{
				Real a;
				a = f(a);
				
				std::sin(a);
			}
			// Conversion count 2.
			TEST_ENSURE_OP(globalStats().statistic<Stat::Enum::Conversions>(), ==, 2);
		}

	};

	void testStatReal()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("StatReal", testStatReal);
	}

	CallFunction run(addTest);

}
