// Description: Tests whether a type has subtraction.

#ifndef PASTELSYS_IS_SUBTRACTABLE_H
#define PASTELSYS_IS_SUBTRACTABLE_H

#include "pastel/sys/type_traits/compiles.h"
#include "pastel/sys/function/identity_function.h"

namespace Pastel
{

		template <
			typename Type,
			typename = 
				decltype(std::declval<Type>() - std::declval<Type>())
		>
		struct Subtraction_Test {};

		template <typename Type>
		using Is_Subtractable =
			Compiles<Subtraction_Test, Type>;

		template <typename Type>
		using Is_Subtractable_F =
			Identity_F<Is_Subtractable<Type>>;

}

#endif
