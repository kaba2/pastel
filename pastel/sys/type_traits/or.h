// Description: Logical Or type-function

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
		struct Or_F
			: std::integral_constant<bool, 
			Left_Bool::value || Right_Bool::value>
		{};

	}


	template <typename... BoolSet>
	using Or_F = 
		Fold<Or_::Or_F, BoolSet..., std::false_type>;

	template <typename... BoolSet>
	using Or = 
		typename Or_F<BoolSet...>::type;

}

#endif
