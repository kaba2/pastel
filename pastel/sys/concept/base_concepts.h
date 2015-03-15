// Description: Gathers base-concepts to Refines<...>

#ifndef PASTELSYS_BASE_CONCEPTS_H
#define PASTELSYS_BASE_CONCEPTS_H

#include "pastel/sys/concept/refines.h"
#include "pastel/sys/concept/join_refines.h"
#include "pastel/sys/type_traits/is_template_instance.h"
#include "pastel/sys/type_traits/template_base.h"

namespace Pastel
{

	namespace Concept_
	{

		template <typename Concept>
		struct BaseConcepts_F_
		{
		private:
			using preType =
				typename std::conditional<
					IsTemplateInstance<Concept, Refines>::value,
					void,
					TemplateBase<Refines, Concept>
				>::type;
			
		public:
			using type =
				typename std::conditional<
					std::is_same<preType, void>::value,
					Refines<>,
					preType
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
	struct BaseConcepts_F
	{
		using type = 
			JoinRefines<typename Concept_::BaseConcepts_F_<ConceptSet>::type...>;
	};

	template <typename... ConceptSet>
	using BaseConcepts =
		typename BaseConcepts_F<ConceptSet...>::type;

}

#endif
