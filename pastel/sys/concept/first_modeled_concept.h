// Description: Search for the first modeled concept

#ifndef PASTELSYS_FIRST_MODELED_CONCEPT_H
#define PASTELSYS_FIRST_MODELED_CONCEPT_H

#include "pastel/sys/concept/models.h"

#include <type_traits>

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
	struct FirstModeledConcept;

	template <
		typename Type,
		typename Concept,
		typename... ConceptSet>
	struct FirstModeledConcept<Type, Concept, ConceptSet...>
	{
		using type = 
			typename std::conditional<
				Models<Type, Concept>::value,
				Concept,
				typename FirstModeledConcept<Type, ConceptSet...>::type
			>::type;
	};

	template <typename Type>
	struct FirstModeledConcept<Type>
	{
		using type = void;
	};

	PASTEL_TYPE_FUNCTION(FirstModeledConcept);

}

#endif
