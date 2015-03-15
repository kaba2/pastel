// Description: Fold types with a binary type-function

#ifndef PASTELSYS_FOLD_H
#define PASTELSYS_FOLD_H

namespace Pastel
{

	template <
		template <typename, typename> class Binary_TypeFunction,
		typename... TypeSet>
	struct Fold_F;

	template <
		template <typename, typename> class Binary_TypeFunction,
		typename Type,
		typename... TypeSet>
	struct Fold_F<Binary_TypeFunction, Type, TypeSet...>
	{
		using type = 
			typename Binary_TypeFunction<
				Type,
				typename Fold_F<Binary_TypeFunction, TypeSet...>::type
			>::type;
	};

	template <
		template <typename, typename> class Binary_TypeFunction,
		typename Type>
	struct Fold_F<Binary_TypeFunction, Type>
	{
		using type = Type;
	};

	template <template <typename, typename> class Binary_TypeFunction>
	struct Fold_F<Binary_TypeFunction>
	{
		using type = void;
	};

}

namespace Pastel
{

	template <
		template <typename, typename> class Binary_TypeFunction,
		typename... TypeSet>
	using Fold = 
		typename Fold_F<Binary_TypeFunction, TypeSet...>::type;

}

#endif
