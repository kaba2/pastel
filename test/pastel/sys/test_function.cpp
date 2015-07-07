// Description: Testing for functions
// Documentation: unit_testing.txt

#include "test/test_init.h"

#include "pastel/sys/function.h"

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			testFirstAndSecond();
		}

		void testArchetype()
		{
			{
				using F = Function_Archetype<void, integer>;
				PASTEL_CONCEPT_CHECK(F, Function_Concept<void>(integer));
			}
			{
				using F = Function_Archetype<integer, integer>;
				PASTEL_CONCEPT_CHECK(F, Function_Concept<integer>(integer));
			}
		}

		void testFirstAndSecond()
		{
			using Pair = std::pair<integer, real>;
			PASTEL_STATIC_ASSERT(std::is_same<decltype(First_Function()(Pair())), integer&>::value);
			PASTEL_STATIC_ASSERT(!std::is_same<decltype(First_Function()(Pair())), real&>::value);
			PASTEL_STATIC_ASSERT(std::is_same<decltype(Second_Function()(Pair())), real&>::value);
			PASTEL_STATIC_ASSERT(!std::is_same<decltype(Second_Function()(Pair())), integer&>::value);

			PASTEL_CONCEPT_CHECK(First_Function, Function_Concept<integer>(Pair));
			PASTEL_CONCEPT_CHECK(First_Function, Function_Concept<integer&>(Pair));
			PASTEL_CONCEPT_CHECK(First_Function, Function_Concept<const integer&>(Pair));

			PASTEL_CONCEPT_CHECK(First_Function, Function_Concept<integer>(Pair&));
			PASTEL_CONCEPT_CHECK(First_Function, Function_Concept<integer&>(Pair&));
			PASTEL_CONCEPT_CHECK(First_Function, Function_Concept<const integer&>(Pair&));

			PASTEL_CONCEPT_CHECK(First_Function, Function_Concept<integer>(const Pair&));
			PASTEL_CONCEPT_REJECT(First_Function, Function_Concept<integer&>(const Pair&));
			PASTEL_CONCEPT_CHECK(First_Function, Function_Concept<const integer&>(const Pair&));

			PASTEL_CONCEPT_CHECK(First_Function, Function_Concept<real>(Pair));
			PASTEL_CONCEPT_REJECT(First_Function, Function_Concept<real&>(Pair));
			PASTEL_CONCEPT_CHECK(First_Function, Function_Concept<const real&>(Pair));

			PASTEL_CONCEPT_CHECK(Second_Function, Function_Concept<real>(Pair));
			PASTEL_CONCEPT_CHECK(Second_Function, Function_Concept<real&>(Pair));
			PASTEL_CONCEPT_REJECT(Second_Function, Function_Concept<integer&>(Pair));
		}
	};

	TEST_CASE("function", "[function]")
	{
	}

}