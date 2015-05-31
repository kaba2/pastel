// Description: Testing for native reals
// DocumentationOf: native_real.h

#include "test_pastelsys.h"

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
			test<float>();
			test<double>();
			test<long double>();
		}

		template <typename Type>
		void test()
		{
			PASTEL_CONCEPT_CHECK(Type, Real_Concept);

			TEST_ENSURE(isInfinity(infinity<Type>()));
			TEST_ENSURE(isMinusInfinity(-infinity<Type>()));
			TEST_ENSURE(isNan(nan<Type>()));
			TEST_ENSURE(inverse((Type)5) == 1 / (Type)5);
			
			TEST_ENSURE(negative((Type)-1));
			TEST_ENSURE(!negative((Type)1));
			TEST_ENSURE(!negative((Type)0));
			
			TEST_ENSURE(positive((Type)1));
			TEST_ENSURE(!positive((Type)-1));
			TEST_ENSURE(!positive((Type)0));
			
			TEST_ENSURE(zero((Type)0));
			TEST_ENSURE(zero(-(Type)0));
			TEST_ENSURE(!zero((Type)1));
			TEST_ENSURE(!zero((Type)-1));
			
			TEST_ENSURE(floor((Type)0.5) == 0);
			TEST_ENSURE(floor((Type)-0.5) == -1);
			
			TEST_ENSURE(ceil((Type)0.5) == 1);
			TEST_ENSURE(ceil((Type)-0.5) == 0);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("native_real", test);
	}

	CallFunction run(addTest);

}
