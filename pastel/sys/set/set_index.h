#ifndef PASTELSYS_SET_INDEX_H
#define PASTELSYS_SET_INDEX_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/type_traits/remove_cvref.h"
#include "pastel/sys/function/identity_function.h"

namespace Pastel
{

	template <
		typename Set,
		Requires<
			Models<Set, Set_Concept>
		> = 0
	>
	using Set_Index = 
		typename ranges::iterator_t<Set>;
	
	template <
		typename Set,
		Requires<
			Models<Set, Set_Concept>
		> = 0
	>
	using Set_Index_F = 
		Identity_F<Set_Index<Set>>;

}

#endif
