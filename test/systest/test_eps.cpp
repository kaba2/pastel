// Description: Testing for adjacent floating point numbers
// DocumentationOf: eps.h

#include "pastelsystest.h"

#include "pastel/sys/eps.h"

using namespace Pastel;

namespace
{

	class Eps_Test
		: public TestSuite
	{
	public:
		Eps_Test()
			: TestSuite(&sysTestReport())
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

			TEST_ENSURE_OP(nextGreater((Type)1), >, 1);
			TEST_ENSURE_OP(nextSmaller((Type)1), <, 1);
			TEST_ENSURE_OP(nextGreater((Type)0), >, 0);
			TEST_ENSURE_OP(nextSmaller((Type)0), <, 0);
			TEST_ENSURE_OP(nextGreater((Type)minusZero), >, 0);
			TEST_ENSURE_OP(nextSmaller((Type)minusZero), <, 0);
			TEST_ENSURE_OP(nextGreater((Type)-1), >, -1);
			TEST_ENSURE_OP(nextSmaller((Type)-1), <, -1);
			TEST_ENSURE_OP(nextGreater(infinity<Type>()), ==, infinity<Type>());
			TEST_ENSURE_OP(nextSmaller(infinity<Type>()), ==, infinity<Type>());
			TEST_ENSURE_OP(nextGreater(-infinity<Type>()), ==, -infinity<Type>());
			TEST_ENSURE_OP(nextSmaller(-infinity<Type>()), ==, -infinity<Type>());
			TEST_ENSURE(StdExt::isNan(nextSmaller(nan<Type>())));
		}

		void test()
		{
			testCase<real64>();
			testCase<real32>();
		}
	};

	void testEps()
	{
		Eps_Test test;
		test.run();
	}

	void addTests()
	{
		sysTestList().add("eps", testEps);
	}

	CallFunction run(addTests);

}
