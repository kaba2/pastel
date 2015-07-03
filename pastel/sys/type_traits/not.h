// Description: Logical-not type-function
// Documentation: type_traits.txt

#ifndef PASTELSYS_NOT_H
#define PASTELSYS_NOT_H

#include "pastel/sys/type_traits/identity.h"

namespace Pastel
{

	template <typename Condition>
	struct Not_F
	: Identity_F<
		std::integral_constant<bool, !Condition::value>
	>
	{};

	template <typename Condition>
	using Not = 
		typename Not_F<Condition>::type;

}

#endif
