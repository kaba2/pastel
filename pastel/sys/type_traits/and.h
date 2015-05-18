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
			typename Left_Bool,
			typename Right_Bool>
		using And_F_ =
			Identity_F<
				BoolConstant<
					Left_Bool::value && Right_Bool::value
				>
			>;

	}

	template <typename... BoolSet>
	using And =
		Fold<And_::And_F_, std::true_type, BoolSet...>;

	template <typename... BoolSet>
	using And_F = 
		Identity_F<And<BoolSet...>>;

}

#endif
