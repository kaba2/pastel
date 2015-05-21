// Description: Logical-or type-function
// Documentation: type_traits.txt

#ifndef PASTELSYS_OR_H
#define PASTELSYS_OR_H

#include "pastel/sys/type_traits/fold.h"
#include "pastel/sys/mytypes.h"

#include <type_traits>

namespace Pastel
{

	namespace Or_
	{

		template <
			typename Left_Bool,
			typename Right_Bool>
		using Or_F_ =
			Identity_F<
				BoolConstant< 
					Left_Bool::value || Right_Bool::value
				>
			>;

	}

	template <typename... BoolSet>
	using Or =
		Fold<Or_::Or_F_, std::false_type, BoolSet...>;

	template <typename... BoolSet>
	using Or_F = 
		Identity_F<Or<BoolSet...>>;

}

#endif
