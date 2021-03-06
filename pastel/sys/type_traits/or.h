// Description: Logical-or type-function
// Documentation: type_traits.txt

#ifndef PASTELSYS_OR_H
#define PASTELSYS_OR_H

#include "pastel/sys/type_traits/fold.h"

#include <type_traits>

namespace Pastel
{

	namespace Or_
	{

		template <
			typename Left_Condition,
			typename Right_Condition>
		using Or_F_ =
			Identity_F<
				std::bool_constant<Left_Condition::value || Right_Condition::value>
			>;

	}

	template <typename... ConditionSet>
	using Or =
		Fold<Or_::Or_F_, std::false_type, ConditionSet...>;

}

#endif
