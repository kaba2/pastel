// Description: Testing for indicators
// DocumentationOf: indicator.h

#include "test_pastelsys.h"

#include <pastel/sys/indicator.h>

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
			struct A {};

			PASTEL_CONCEPT_CHECK(All_Indicator, Indicator_Concept(integer));
			PASTEL_CONCEPT_CHECK(All_Indicator, Indicator_Concept(A));

			PASTEL_CONCEPT_CHECK(Complement_Indicator<All_Indicator>, Indicator_Concept(integer));
			PASTEL_CONCEPT_CHECK(Complement_Indicator<All_Indicator>, Indicator_Concept(A));

			PASTEL_CONCEPT_CHECK(Empty_Indicator, Indicator_Concept(integer));
			PASTEL_CONCEPT_CHECK(Empty_Indicator, Indicator_Concept(A));
			
			{
				using Indicator = Predicate_Indicator<integer, LessThan>;
				PASTEL_CONCEPT_CHECK(Indicator, Indicator_Concept(integer));
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
		testRunner().add("indicator", test);
	}

	CallFunction run(addTest);

}
