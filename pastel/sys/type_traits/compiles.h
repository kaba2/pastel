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
	A class/alias template whose instantiation 
	Code<TypeSet...> encapsulates the code to 
	be tested.

	TypeSet:
	The arguments of Code.
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

	//! Computes a compile-time function, if it compiles.
	/*!
	Otherwise returns a fail-type.

	FailType:
	The type to return in case Code<TypeSet...>
	does not compile.

	Code:
	A class/alias template such that Code<TypeSet..>
	provides the result of the computation, if
	it compiles.

	TypeSet:
	The set of arguments to Code.
	*/
	template <
		typename FailType,
		template <typename...> class Code,
		typename... TypeSet
	>
	struct Compute_F
	{
	private:
		template <
			typename... T,
			typename Return = Code<T...>
			>
		static Return test(T&&...);
		
		static FailType test(...);

	public:
		using type = 
			decltype(test(std::declval<TypeSet>()...));
	};

	template <
		typename FailType,
		template <typename...> class Code,
		typename... TypeSet
	>
	using Compute = 
		typename Compute_F<FailType, Code, TypeSet...>::type;

}

#endif
