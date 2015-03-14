// Description: Check direct requirements of a concept

#ifndef PASTELSYS_MODELS_DIRECTLY_H
#define PASTELSYS_MODELS_DIRECTLY_H

#include <type_traits>

namespace Pastel
{

	//! Checks direct requirements of a concept.
	/*!
	The requirements of base-concepts will not 
	be checked.
	*/
	template <
		typename Type, 
		typename Concept>
	struct Models_Directly
	{
		// The concept is not in functional form.
		// Convert it to such, assuming no parameters.
		static PASTEL_CONSTEXPR bool value =
			Models_Directly<Type, Concept()>::value;
	};

	template <
		typename Type, 
		typename Concept,
		typename... ParameterSet>
	struct Models_Directly<Type, Concept(ParameterSet...)>
	{
	private:
		template <
			typename T,
			typename = decltype(std::declval<Concept>().requires(
				std::declval<T>(), std::declval<ParameterSet>()...))>
		static std::true_type test();
		
		template <typename T>
		static std::false_type test(...);

	public:
		static PASTEL_CONSTEXPR bool value =
			decltype(test<Type>())::value;
	};

}

#endif
