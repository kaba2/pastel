// Description: Converts a type-function SFINAE-friendly

#ifndef PASTELSYS_SFINAE_BINARY_REDUCE_H
#define PASTELSYS_SFINAE_BINARY_REDUCE_H

#include <type_traits>

namespace Pastel
{

	namespace Sfinae_Binary_Reduce_
	{

		// Using alias template here crashes Visual Studio 14 CTP4.
		template <
			template <typename...> class F, 
			typename... TypeSet>
		struct At : F<TypeSet...> {};

		template <typename Type>
		using Void = typename std::conditional<true, void, Type>::type;

		template <
			template <typename...> class,
			typename,
			typename...>
		struct Reduce 
		{
		};

		template <
			template <typename...> class F>
		struct Reduce<F, Void<typename At<F>::type>>
		{
			// Nullary reduction.
			using type = typename At<F>::type;
		};

		template <
			template <typename...> class F, 
			typename A_Type>
		struct Reduce<F, Void<typename At<F, A_Type>::type>, A_Type>
		{
			// Unary reduction.
			using type = typename At<F, A_Type>::type;
		};

		template <
			template <typename...> class F,
			typename A_Type, 
			typename B_Type,
			typename... TypeSet>
		struct Reduce<F, Void<typename At<F, A_Type, B_Type>::type>, A_Type, B_Type, TypeSet...>
		{
			// N-ary reduction by chained binary reduction.
			using type = 
				typename Reduce<F, void, typename At<F, A_Type, B_Type>::type, TypeSet...>::type;
		};

	}

	template <
		template <typename...> class F, 
		typename... TypeSet>
	using Sfinae_Binary_Reduce = 
		typename Sfinae_Binary_Reduce_::Reduce<F, void, TypeSet...>::type;

}

#endif
