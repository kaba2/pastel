// Description: Checks whether a type models a concept

#ifndef PASTELSYS_MODELS_H
#define PASTELSYS_MODELS_H

#include "pastel/sys/concept/refines.h"
#include "pastel/sys/concept/models_directly.h"

#include <type_traits>

namespace Pastel
{

	//! Checks whether a type models the given concepts.
	template <
		typename Type, 
		typename... ConceptSet>
	struct Models;

	//! Checks whether a type models base-concepts.
	/*!
	The direct requirements of the concept will 
	not be checked.
	*/
	template <
		typename Type, 
		typename Concept>
	struct Models_Base
	{
	private:
		template <typename... ConceptSet>
		static auto test(Refines<ConceptSet...>&&)
		-> std::integral_constant<bool, Models<Type, ConceptSet...>::value>;
		
		// The concept does not refine other concepts.
		static std::true_type test(...);

	public:
		static PASTEL_CONSTEXPR bool value =
			decltype(test(std::declval<Concept>()))::value;
	};

}

namespace Pastel
{

	template <
		typename Type, 
		typename Concept, 
		typename... ConceptSet>
	struct Models<Type, Concept, ConceptSet...>
	{
		static PASTEL_CONSTEXPR bool value =
			Models_Directly<Type, Concept>::value &&
			Models_Base<Type, Concept>::value &&
			Models<Type, ConceptSet...>::value;
	};

	template <typename Type>
	struct Models<Type>
		: std::true_type
	{};

}

namespace Pastel
{

	template <
		typename Concept, 
		typename Type,
		bool Value = Models<Type, Concept>::value>
	PASTEL_CONSTEXPR bool isModelOf(Type&& that)
	{
		return Value;
	}

}

#endif
