// Description: Joins Refines<...> structures together.

#ifndef PASTELSYS_JOIN_REFINES_H
#define PASTELSYS_JOIN_REFINES_H

#include "pastel/sys/concept/refines.h"
#include "pastel/sys/type_traits/fold.h"

namespace Pastel
{

	namespace JoinRefines_
	{

		template <
			typename A_Refines,
			typename B_Refines>
		struct JoinRefines_F_;

		template <
			typename... A_ConceptSet,
			typename... B_ConceptSet>
		struct JoinRefines_F_<
			Refines<A_ConceptSet...>, 
			Refines<B_ConceptSet...>>
		{
			using type = 
				Refines<
					A_ConceptSet..., 
					B_ConceptSet...
				>;
		};

	}

	template <typename... RefinesSet>
	struct JoinRefines_F
	{
		using type =
			Fold<JoinRefines_::JoinRefines_F_, Refines<>, RefinesSet...>;
	};

	template <typename... RefinesSet>
	using JoinRefines = 
		typename JoinRefines_F<RefinesSet...>::type;

}

#endif
