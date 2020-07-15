// Description: Tests whether a type has subtraction.

#ifndef PASTELSYS_IS_SUBTRACTABLE_H
#define PASTELSYS_IS_SUBTRACTABLE_H

#include "pastel/sys/type_traits.h"

namespace Pastel
{

		template <typename T>
		concept Is_Subtractable__ = requires(T t) {
			t - t;
		};

		template <typename T>
		concept Is_Subtractable_ = 
			Is_Subtractable__<RemoveCvRef<T>>;

		template <typename Type>
		using Is_Subtractable = 
			std::bool_constant<Is_Subtractable_<Type>>;

}

#endif
