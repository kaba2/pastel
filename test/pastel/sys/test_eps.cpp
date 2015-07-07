// Description: Testing for adjacent floating point numbers
// DocumentationOf: eps.h

#include "test/test_init.h"

#include "pastel/sys/math/eps.h"

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			test();
		}

		template <typename Type>
		void testCase()
		{
			const Type minusZero = (Type)-1 * 0;

			REQUIRE(minusZero == 0);
			REQUIRE(nextGreater((Type)1) > 1);
			REQUIRE(nextSmaller((Type)1) < 1);
			REQUIRE(nextGreater((Type)0) > 0);
			REQUIRE(nextSmaller((Type)0) < 0);
			REQUIRE(nextGreater(minusZero) > 0);
			REQUIRE(nextSmaller(minusZero) < 0);
			REQUIRE(nextGreater((Type)-1) > -1);
			REQUIRE(nextSmaller((Type)-1) < -1);
			REQUIRE(nextGreater(infinity<Type>()) == infinity<Type>());
			REQUIRE(nextSmaller(infinity<Type>()) == infinity<Type>());
			REQUIRE(nextGreater(-infinity<Type>()) == -infinity<Type>());
			REQUIRE(nextSmaller(-infinity<Type>()) == -infinity<Type>());
			REQUIRE(isNan(nextSmaller(nan<Type>())));
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

}
