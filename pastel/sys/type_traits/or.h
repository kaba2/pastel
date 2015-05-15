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
			typename Left_Bool,
			typename Right_Bool>
		struct Or_F_
		: Identity_F<std::integral_constant<bool, 
			Left_Bool::value || Right_Bool::value>
		>
		{};

	}

	template <typename... BoolSet>
	struct Or_F
	: Fold_F<Or_::Or_F_, std::false_type, BoolSet...>
	{};

	template <typename... BoolSet>
	using Or = 
		typename Or_F<BoolSet...>::type;

}

#endif
