// Descriptions: Find the most refined concept modeled by a type

#ifndef PASTELSYS_MOST_REFINED_CONCEPT_H
#define PASTELSYS_MOST_REFINED_CONCEPT_H

#include "pastel/sys/concept/concept.h"
#include "pastel/sys/concept/first_modeled_concept.h"
#include "pastel/sys/concept/base_concepts.h"

namespace Pastel
{

	namespace Concept_
	{

		template <
			typename Type,
			typename... ConceptSet>
		struct MostRefinedConcept_F
		{
		private:
			using ModeledConcept =
				FirstModeledConcept<
					Type, ConceptSet...
				>;

		public:
			template <typename>
			struct BreadthFirst;

			template <typename... BaseSet>
			struct BreadthFirst<Refines<BaseSet...>>
			{
				using type =
					typename MostRefinedConcept_F<
						Type, BaseSet...
					>::type;
			};

			using BaseSet =
				BaseConcepts<ConceptSet...>;

			using type =
				typename std::conditional<
					std::is_same<ModeledConcept, void>::value,
					typename BreadthFirst<BaseSet>::type,
					ModeledConcept
				>::type;
		};

		template <typename Type>
		struct MostRefinedConcept_F<Type>
		{
			using type = void;
		};

	}

	//! Returns the most refined concept modeled by Type.
	/*!
	This is a breadth-first search over the Refines<...>
	inheritance hierarchy. Sequences (i.e. arguments of 
	Refines<...>) are searched from first to last.

	returns:
	The first matching concept. If there is none,
	then void.
	*/
	template <
		typename Type,
		typename Concept>
	using MostRefinedConcept_F =
		Concept_::MostRefinedConcept_F<Type, Concept>;

	template <
		typename Type,
		typename Concept>
	using MostRefinedConcept =
		typename MostRefinedConcept_F<Type, Concept>::type;

}

#endif
