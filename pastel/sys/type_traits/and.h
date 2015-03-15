// Description: Logical And type-function

#ifndef PASTELSYS_AND_H
#define PASTELSYS_AND_H

#include "pastel/sys/type_traits/fold.h"

#include <type_traits>

namespace Pastel
{

	namespace And_
	{

		template <
			typename Left_Bool,
			typename Right_Bool>
		struct And_F
			: std::integral_constant<bool, 
			Left_Bool::value && Right_Bool::value>
		{};

	}


	template <typename... BoolSet>
	using And_F = 
		Fold<And_::And_F, BoolSet..., std::true_type>;

	template <typename... BoolSet>
	using And = 
		typename And_F<BoolSet...>::type;

}

#endif
