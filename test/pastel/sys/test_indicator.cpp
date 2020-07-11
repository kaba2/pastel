// Description: Testing for indicators
// Documentation: indicator.txt

#include "test/test_init.h"

#include <pastel/sys/indicator.h>

TEST_CASE("indicator (indicator)")
{
	struct A {};

	static_assert(Indicator_Concept<decltype(allIndicator()), integer>);
	static_assert(Indicator_Concept<All_Indicator, integer>);
	static_assert(Indicator_Concept<All_Indicator, A>);

	static_assert(Indicator_Concept<Complement_Indicator<All_Indicator>, integer>);
	static_assert(Indicator_Concept<Complement_Indicator<All_Indicator>, A>);

	static_assert(Indicator_Concept<Empty_Indicator, integer>);
	static_assert(Indicator_Concept<Empty_Indicator, A>);

	{
		using Indicator = Predicate_Indicator<integer, LessThan>;
		static_assert(Indicator_Concept<Indicator, integer>);
	}
}

