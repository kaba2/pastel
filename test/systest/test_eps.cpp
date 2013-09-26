// Description: Testing for adjacent floating point numbers
// DocumentationOf: eps.h

#include "test_pastelsys.h"

#include "pastel/sys/eps.h"

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

		template <typename Type>
		void testCase()
		{
			const Type minusZero = (Type)-1 * 0;

			TEST_ENSURE_OP(minusZero, == , 0);
			TEST_ENSURE_OP(nextGreater((Type)1), >, 1);
			TEST_ENSURE_OP(nextSmaller((Type)1), <, 1);
			TEST_ENSURE_OP(nextGreater((Type)0), >, 0);
			TEST_ENSURE_OP(nextSmaller((Type)0), <, 0);
			TEST_ENSURE_OP(nextGreater(minusZero), >, 0);
			TEST_ENSURE_OP(nextSmaller(minusZero), <, 0);
			TEST_ENSURE_OP(nextGreater((Type)-1), >, -1);
			TEST_ENSURE_OP(nextSmaller((Type)-1), <, -1);
			TEST_ENSURE_OP(nextGreater(infinity<Type>()), ==, infinity<Type>());
			TEST_ENSURE_OP(nextSmaller(infinity<Type>()), ==, infinity<Type>());
			TEST_ENSURE_OP(nextGreater(-infinity<Type>()), ==, -infinity<Type>());
			TEST_ENSURE_OP(nextSmaller(-infinity<Type>()), ==, -infinity<Type>());
			TEST_ENSURE(isNan(nextSmaller(nan<Type>())));
		}

		void test()
		{
			testCase<real64_ieee>();
			testCase<real32_ieee>();
		}
	};

	void testEps()
	{
		Test test;
		test.run();
	}

	void addTests()
	{
		testRunner().add("eps", testEps);
	}

	CallFunction run(addTests);

}
