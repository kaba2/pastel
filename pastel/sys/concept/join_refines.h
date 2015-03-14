// Description: Joins Refines<...> structures together.

#ifndef PASTELSYS_JOIN_REFINES_H
#define PASTELSYS_JOIN_REFINES_H

#include "pastel/sys/concept/refines.h"

namespace Pastel
{

	template <
		typename A_Refines,
		typename B_Refines>
	struct JoinRefines;

	template <
		typename... A_ConceptSet,
		typename... B_ConceptSet>
	struct JoinRefines<Refines<A_ConceptSet...>, Refines<B_ConceptSet...>>
	{
		using type = Refines<A_ConceptSet..., B_ConceptSet...>;
	};

}

#endif
