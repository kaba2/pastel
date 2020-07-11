// Description: Testing for functions
// Documentation: unit_testing.txt

#include "test/test_init.h"

#include "pastel/sys/function.h"

TEST_CASE("Archetype (function)")
{
	{
		using F = Function_Archetype<void, integer>;
		PASTEL_CONCEPT_CHECK2(F, Function_Concept, void, integer);
		
		F f;
		f(3);
	}
	{
		using F = Function_Archetype<integer, integer>;
		PASTEL_CONCEPT_CHECK2(F, Function_Concept, integer, integer);

		F f;
		integer y = f(3);
	}
}

TEST_CASE("FirstAndSecond (function)")
{
	using Pair = std::pair<integer, dreal>;
	PASTEL_STATIC_ASSERT(std::is_same<decltype(First_Function()(Pair())), integer&>::value);
	PASTEL_STATIC_ASSERT(!std::is_same<decltype(First_Function()(Pair())), dreal&>::value);
	PASTEL_STATIC_ASSERT(std::is_same<decltype(Second_Function()(Pair())), dreal&>::value);
	PASTEL_STATIC_ASSERT(!std::is_same<decltype(Second_Function()(Pair())), integer&>::value);

	PASTEL_CONCEPT_CHECK2(First_Function, Function_Concept, integer, Pair);
	PASTEL_CONCEPT_CHECK2(First_Function, Function_Concept, integer&, Pair);
	PASTEL_CONCEPT_CHECK2(First_Function, Function_Concept, const integer&, Pair);

	PASTEL_CONCEPT_CHECK2(First_Function, Function_Concept, integer, Pair&);
	PASTEL_CONCEPT_CHECK2(First_Function, Function_Concept, integer&, Pair&);
	PASTEL_CONCEPT_CHECK2(First_Function, Function_Concept, const integer&, Pair&);

	PASTEL_CONCEPT_CHECK2(First_Function, Function_Concept, integer, const Pair&);
	PASTEL_CONCEPT_REJECT2(First_Function, Function_Concept, integer&, const Pair&);
	PASTEL_CONCEPT_CHECK2(First_Function, Function_Concept, const integer&, const Pair&);

	PASTEL_CONCEPT_CHECK2(First_Function, Function_Concept, dreal, Pair);
	PASTEL_CONCEPT_REJECT2(First_Function, Function_Concept, dreal&, Pair);
	PASTEL_CONCEPT_CHECK2(First_Function, Function_Concept, const dreal&, Pair);

	PASTEL_CONCEPT_CHECK2(Second_Function, Function_Concept, dreal, Pair);
	PASTEL_CONCEPT_CHECK2(Second_Function, Function_Concept, dreal&, Pair);
	PASTEL_CONCEPT_REJECT2(Second_Function, Function_Concept, integer&, Pair);
}
