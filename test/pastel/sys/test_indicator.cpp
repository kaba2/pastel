// Description: Testing for indicators
// Documentation: indicator.txt

#include "test/test_init.h"

#include <pastel/sys/indicator.h>

TEST_CASE("indicator (indicator)")
{
	struct A {};

	static_assert(Indicator_Concept_<decltype(allIndicator()), integer>);
	static_assert(Indicator_Concept_<All_Indicator, integer>);
	static_assert(Indicator_Concept_<All_Indicator, A>);

	static_assert(Indicator_Concept_<Complement_Indicator<All_Indicator>, integer>);
	static_assert(Indicator_Concept_<Complement_Indicator<All_Indicator>, A>);

	static_assert(Indicator_Concept_<Empty_Indicator, integer>);
	static_assert(Indicator_Concept_<Empty_Indicator, A>);

	{
		using Indicator = Predicate_Indicator<integer, LessThan>;
		static_assert(Indicator_Concept_<Indicator, integer>);
	}
}

