#ifndef PASTELSYS_MOST_REFINED_CONCEPT_H
#define PASTELSYS_MOST_REFINED_CONCEPT_H

#include "pastel/sys/concept/concept.h"
#include "pastel/sys/concept/first_modeled_concept.h"

namespace Pastel
{

	namespace Concept_
	{

		template <
			typename Type,
			typename... ConceptSet>
		struct MostRefinedConcept
		{
		private:
			using ModeledConcept =
				typename FirstModeledConcept<
					Type, ConceptSet...
				>::type;

		public:
			template <typename>
			struct BreadthFirst;

			template <typename... BaseSet>
			struct BreadthFirst<Refines<BaseSet...>>
			{
				using type =
					typename MostRefinedConcept<
						Type, BaseSet...
					>::type;
			};

			using BaseSet =
				typename BaseConcepts<ConceptSet...>::type;

			using type =
				typename std::conditional<
					std::is_same<ModeledConcept, void>::value,
					typename BreadthFirst<BaseSet>::type,
					ModeledConcept
				>::type;
		};

		template <typename Type>
		struct MostRefinedConcept<Type>
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
	struct MostRefinedConcept
	{
		using type = 
			typename Concept_::MostRefinedConcept<Type, Concept>::type;
	};

	template <typename Type>
	struct MostRefinedConcept<Type, void>
	{
		using type = void;
	};

}

#endif
