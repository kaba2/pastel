// Description: Testing for functions
// Documentation: unit_testing.txt

#include "test_pastelsys.h"

#include "pastel/sys/function.h"

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
			using Pair = std::pair<integer, real>;
			Pair a;

			PASTEL_STATIC_ASSERT(std::is_same<decltype(First_Function()(a)), integer&>::value);

			PASTEL_CONCEPT_CHECK(First_Function, Function_Concept(integer, Pair));
			PASTEL_CONCEPT_CHECK(First_Function, Function_Concept(integer&, Pair));
			PASTEL_CONCEPT_REJECT(First_Function, Function_Concept(real&, Pair));

			PASTEL_CONCEPT_CHECK(Second_Function, Function_Concept(real, Pair));
			PASTEL_CONCEPT_CHECK(Second_Function, Function_Concept(real&, Pair));
			PASTEL_CONCEPT_REJECT(Second_Function, Function_Concept(integer&, Pair));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("function", test);
	}

	CallFunction run(addTest);

}
