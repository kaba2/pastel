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

		// Visual Studio 2015 CTP6 has an interesting
		// bug here: using the name And_F instead of And_F_
		// makes the compiler confuse Pastel::And_::And_F
		// with Pastel::And_F.

		template <
			typename Left_Condition,
			typename Right_Condition>
		using And_F_ =
			Identity_F<
				std::integral_constant<
					bool,
					Left_Condition::value && Right_Condition::value
				>
			>;

	}

	template <typename... ConditionSet>
	using And =
		Fold<And_::And_F_, std::true_type, ConditionSet...>;

	template <typename... ConditionSet>
	using And_F = 
		Identity_F<And<ConditionSet...>>;

}

#endif
