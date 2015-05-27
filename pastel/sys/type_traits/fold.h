// Description: Fold types with a binary type-function
// Documentation: type_traits.txt

#ifndef PASTELSYS_FOLD_H
#define PASTELSYS_FOLD_H

#include "pastel/sys/type_traits/identity.h"

namespace Pastel
{

	namespace Fold_
	{

		template <
			template <typename, typename, typename...> class Binary_TypeFunction,
			typename Identity,
			typename... TypeSet>
		struct Fold_F_;

		template <
			template <typename, typename, typename...> class Binary_TypeFunction,
			typename Identity,
			typename Type,
			typename... TypeSet>
		struct Fold_F_<Binary_TypeFunction, Identity, Type, TypeSet...>
		: Binary_TypeFunction<
			Type,
			typename Fold_F_<Binary_TypeFunction, Identity, TypeSet...>::type
		>
		{};

		template <
			template <typename, typename, typename...> class Binary_TypeFunction,
			typename Identity,
			typename Type>
		struct Fold_F_<Binary_TypeFunction, Identity, Type>
		: Identity_F<Type>
		{};

		template <
			template <typename, typename, typename...> class Binary_TypeFunction,
			typename Identity>
		struct Fold_F_<Binary_TypeFunction, Identity>
		: Identity_F<Identity>
		{};

	}

}

namespace Pastel
{

	//! Folds types with a binary type-function.
	/*!
	Identity:
	The identity element of the binary operation. 
	This is returned for the empty fold.
	*/
	template <
		template <typename, typename, typename...> class Binary_TypeFunction,
		typename Identity,
		typename... TypeSet>
	using Fold_F =
		Fold_::Fold_F_<Binary_TypeFunction, Identity, TypeSet...>;

	template <
		template <typename, typename, typename...> class Binary_TypeFunction,
		typename Identity,
		typename... TypeSet>
	using Fold = 
		typename Fold_F<Binary_TypeFunction, Identity, TypeSet...>::type;

}

#endif
