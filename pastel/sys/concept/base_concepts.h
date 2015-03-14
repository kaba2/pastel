// Description: Gathers base-concepts to Refines<...>

#ifndef PASTELSYS_BASE_CONCEPTS_H
#define PASTELSYS_BASE_CONCEPTS_H

#include "pastel/sys/concept/refines.h"
#include "pastel/sys/concept/is_refines_class.h"
#include "pastel/sys/concept/join_refines.h"

namespace Pastel
{

	namespace Concept_
	{

		template <typename Concept>
		struct BaseConcepts
		{
		private:
			template <typename... ConceptSet>
			static Refines<ConceptSet...> test(Refines<ConceptSet...>&&);
			
			static Refines<> test(...);

		public:
			using type = 
				typename std::conditional<
					IsRefinesClass<Concept>::value,
					void,
					decltype(test(std::declval<Concept>()))
				>::type;
		};

	}

	//! Retrieves the base-concepts.
	/*!
	returns:
	The base-concepts of each concept combined 
	into a Refine<...> class.
	*/
	template <typename... ConceptSet>
	struct BaseConcepts;

	template <
		typename Concept,
		typename... ConceptSet>
	struct BaseConcepts<Concept, ConceptSet...>
	{
		using type = 
			typename JoinRefines<
				typename Concept_::BaseConcepts<Concept>::type,
				typename BaseConcepts<ConceptSet...>::type
			>::type;
	};

	template <>
	struct BaseConcepts<>
	{
		using type = Refines<>;
	};

}

#endif
