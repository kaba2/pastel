// Description: Fold types with a binary type-function

#ifndef PASTELSYS_FOLD_H
#define PASTELSYS_FOLD_H

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
		{
			using type = 
				typename Binary_TypeFunction<
					Type,
					typename Fold_F_<Binary_TypeFunction, Identity, TypeSet...>::type
				>::type;
		};

		template <
			template <typename, typename, typename...> class Binary_TypeFunction,
			typename Identity,
			typename Type>
		struct Fold_F_<Binary_TypeFunction, Identity, Type>
		{
			using type = Type;
		};

		template <
			template <typename, typename, typename...> class Binary_TypeFunction,
			typename Identity>
		struct Fold_F_<Binary_TypeFunction, Identity>
		{
			using type = Identity;
		};

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
	struct Fold_F
	{
		using type = 
			typename Fold_::Fold_F_<Binary_TypeFunction, Identity, TypeSet...>::type;
	};

	template <
		template <typename, typename, typename...> class Binary_TypeFunction,
		typename Identity,
		typename... TypeSet>
	using Fold = 
		typename Fold_F<Binary_TypeFunction, Identity, TypeSet...>::type;

}

#endif
