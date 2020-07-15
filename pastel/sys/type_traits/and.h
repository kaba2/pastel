// Description: Logical-and type-function
// Documentation: type_traits.txt

#ifndef PASTELSYS_AND_H
#define PASTELSYS_AND_H

#include "pastel/sys/type_traits/fold.h"

#include <type_traits>

namespace Pastel
{

	namespace And_
	{

		template <
			typename Left_Condition,
			typename Right_Condition>
		using And_F_ =
			Identity_F<
				std::bool_constant<Left_Condition::value && Right_Condition::value>
			>;

	}

	template <typename... ConditionSet>
	using And =
		Fold<And_::And_F_, std::true_type, ConditionSet...>;

}

#endif
