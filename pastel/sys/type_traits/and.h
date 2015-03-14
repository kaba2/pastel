// Description: Logical And type-function

#ifndef PASTELSYS_AND_H
#define PASTELSYS_AND_H

#include <type_traits>

namespace Pastel
{

	template <typename... BoolSet>
	struct And;

	template <typename Bool, typename... BoolSet>
	struct And<Bool, BoolSet...>
		: std::integral_constant<bool, Bool::value && And<BoolSet...>::value>
	{};

	template <>
	struct And<>
		: std::true_type
	{};

}

#endif
