// Description: Search for the first modeled concept

#ifndef PASTELSYS_FIRST_MODELED_CONCEPT_H
#define PASTELSYS_FIRST_MODELED_CONCEPT_H

#include "pastel/sys/concept/models.h"

#include <type_traits>

namespace Pastel
{

	namespace FirstModeledConcept_
	{

		template <
			typename Type,
			typename... ConceptSet>
		struct FirstModeledConcept_F;

		template <
			typename Type,
			typename Concept,
			typename... ConceptSet>
		struct FirstModeledConcept_F<Type, Concept, ConceptSet...>
		: std::conditional<
			Models<Type, Concept>::value,
			Concept,
			typename FirstModeledConcept_F<Type, ConceptSet...>::type
		>
		{};

		template <typename Type>
		struct FirstModeledConcept_F<Type>
		: Identity_F<void>
		{};

	}

}

namespace Pastel
{

	//! Returns the first modeled concept.
	/*!
	returns:
	The first concept in ConceptSet whose model Type is.
	If there is no such concept, then void.
	*/
	template <
		typename Type,
		typename... ConceptSet>
	using FirstModeledConcept_F =
		FirstModeledConcept_::FirstModeledConcept_F<Type, ConceptSet...>;

	template <
		typename Type,
		typename... ConceptSet>
	using FirstModeledConcept =
		typename FirstModeledConcept_F<Type, ConceptSet...>::type;

}

#endif
