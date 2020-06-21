// Description: Tests whether a type has addition.

#ifndef PASTELSYS_IS_ADDABLE_H
#define PASTELSYS_IS_ADDABLE_H

#include "pastel/sys/type_traits.h"

namespace Pastel
{

		template <typename T>
		concept Is_Addable__ = requires(T t) {
			t + t;			
		};

		template <typename T>
		concept Is_Addable_ = 
			Is_Addable__<RemoveCvRef<T>>;

		template <typename Type>
		using Is_Addable =
			std::bool_constant<Is_Addable_<Type>>;

		template <typename Type>
		using Is_Addable_F =
			Identity_F<Is_Addable<Type>>;

}

#endif
