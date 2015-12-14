// Description: Test whether well-formed code compiles

#ifndef PASTELSYS_COMPILES_H
#define PASTELSYS_COMPILES_H

#include <type_traits>
#include <utility>

namespace Pastel
{

	//! Tests whether well-formed code compiles.
	/*!
	Code:
	A type-function, whose computation encapsulates
	the code to be tested.

	TypeSet:
	The arguments of the type-function.
	*/
	template <
		template <typename...> class Code,
		typename... TypeSet
	>
	struct Compiles
	{
	private:
		template <
			typename... T,
			typename = Code<T...>
			>
		static std::true_type test(T&&...);
		
		static std::false_type test(...);

	public:
		static constexpr bool value =
			decltype(test(std::declval<TypeSet>()...))::value;
	};

}

#endif
