// Description: Logical-not type-function
// Documentation: type_traits.txt

#ifndef PASTELSYS_NOT_H
#define PASTELSYS_NOT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/type_traits/identity.h"

namespace Pastel
{

	template <typename Bool>
	struct Not_F
	: Identity_F<BoolConstant<!Bool::value>>
	{};

	template <typename Bool>
	using Not = 
		typename Not_F<Bool>::type;

}

#endif
