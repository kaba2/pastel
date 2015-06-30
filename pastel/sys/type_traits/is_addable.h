// Description: Tests whether a type has addition.

#ifndef PASTELSYS_IS_ADDABLE_H
#define PASTELSYS_IS_ADDABLE_H

#include "pastel/sys/type_traits/compiles.h"
#include "pastel/sys/function/identity_function.h"

namespace Pastel
{

		template <
			typename Type,
			typename = 
				decltype(std::declval<Type>() + std::declval<Type>())
		>
		class Addition_Test {};

		template <typename Type>
		using Is_Addable =
			Compiles<Addition_Test, Type>;

		template <typename Type>
		using Is_Addable_F =
			Identity_F<Is_Addable<Type>>;

}

#endif
