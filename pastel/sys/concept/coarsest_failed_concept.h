// Description: Find the coarsest concepts not modeled by a type
// Documentation: concept.txt

#ifndef PASTELSYS_COARSEST_FAILED_CONCEPT_H
#define PASTELSYS_COARSEST_FAILED_CONCEPT_H

#include "pastel/sys/concept/concept.h"
#include "pastel/sys/concept/base_concepts.h"
#include "pastel/sys/concept/join_refines.h"

namespace Pastel
{

	namespace Concept_
	{

		template <
			typename Type,
			typename Concept>
		struct CoarsestFailedConcept_F_;

		template <
			typename Type,
			typename Concept,
			typename... ConceptSet>
		struct CoarsestFailedConcept_F_<Type, Refines<Concept, ConceptSet...>>
		: JoinRefines_F<
			std::conditional_t<
				Models_Directly<Type, Concept>::value,
				typename CoarsestFailedConcept_F_<
					Type, BaseConcepts<Concept>
				>::type,
				Refines<Concept>
			>,
			typename CoarsestFailedConcept_F_<
				Type,
				Refines<ConceptSet...>
			>::type
		>
		{};

		template <typename Type>
		struct CoarsestFailedConcept_F_<Type, Refines<>>
		: Identity_F<Refines<>>
		{};

	}

	//! Returns the coarsests concepts not modeled by Type.
	/*!
	Replaces each directly-satisfied concept with its base-concepts,
	until a fixed-point is reached.

	returns:
	A Refines<...> class which lists all the unsatisfied
	concepts.
	*/
	template <
		typename Type,
		typename... ConceptSet>
	struct CoarsestFailedConcept_F
	: Concept_::CoarsestFailedConcept_F_<Type, Refines<ConceptSet...>>
	{};

	template <
		typename Type,
		typename... ConceptSet>
	using CoarsestFailedConcept =
		typename CoarsestFailedConcept_F<Type, ConceptSet...>::type;

}

#endif
