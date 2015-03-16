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
		-> Models<Type, ConceptSet...>;
		
		// The concept does not refine other concepts.
		static std::true_type test(...);

	public:
		static PASTEL_CONSTEXPR bool value =
			decltype(test(std::declval<Concept>()))::value;
	};

}

namespace Pastel
{

	namespace Models_
	{

		template <
			typename Type, 
			typename Concept>
		struct Models_
			: And<
				Models_Directly<Type, Concept>,
				Models_Base<Type, Concept>
			>
		{};

	}

	template <
		typename Type, 
		typename... ConceptSet>
	struct Models
		: And<Models_::Models_<Type, ConceptSet>...>
	{};

}

namespace Pastel
{

	namespace Concept
	{

		template <
			typename Concept_, 
			typename Type
			>
		auto isModelOf(Type&& that) -> 
			EnableIf<Models<Type, Concept_>>;

	}


}

#endif
