// Description: Checks whether a type models a concept

#ifndef PASTELSYS_MODELS_H
#define PASTELSYS_MODELS_H

#include "pastel/sys/concept/refines.h"
#include "pastel/sys/concept/models_directly.h"
#include "pastel/sys/type_traits/and.h"

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
		-> Models<Type, ConceptSet...>;
		
		// The concept does not refine other concepts.
		static std::true_type test(...);

	public:
		static constexpr bool value =
			decltype(test(std::declval<Concept>()))::value;
	};

}

namespace Pastel
{

	template <
		typename Type, 
		typename... ConceptSet>
	struct Models
	{
		static constexpr bool value = 
			And<
				Models_Directly<Type, ConceptSet>...,
				Models_Base<Type, ConceptSet>...
			>::value;
	};

}

namespace Pastel
{

	namespace Concept
	{

		template <
			typename Concept_, 
			typename Type,
			Requires<Models<Type, Concept_>> = 0
			>
		bool isModelOf(Type&& that);

	}

}

#endif
