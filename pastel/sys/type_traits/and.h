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
			typename Left_Bool,
			typename Right_Bool>
		struct And_F_
		{
			// Visual Studio 2015 CTP6 has an interesting
			// bug here: using the name And_F instead of And_F_
			// makes the compiler confuse Pastel::And_::And_F
			// with Pastel::And_F.

			using type =
				std::integral_constant<bool,
				Left_Bool::value && Right_Bool::value>;
		};

	}

	template <typename... BoolSet>
	struct And_F
	{
		using type = 
			Fold<And_::And_F_, std::true_type, BoolSet...>;
	};

	template <typename... BoolSet>
	using And = 
		typename And_F<BoolSet...>::type;

}

#endif
