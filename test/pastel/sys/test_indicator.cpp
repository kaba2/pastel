// Description: Testing for indicators
// Documentation: indicator.txt

#include "test/test_init.h"

#include <pastel/sys/indicator.h>

TEST_CASE("indicator (indicator)")
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

