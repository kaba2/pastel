// Description: Type of elements in a set

#ifndef PASTELSYS_SET_ELEMENT_H
#define PASTELSYS_SET_ELEMENT_H

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
	using Set_Element = 
		typename RemoveCvRef<Set>::Element;
	
	template <
		typename Set,
		Requires<
			Models<Set, Set_Concept>
		> = 0>
	using Set_Element_F = 
		Identity_F<Set_Element<Set>>;

}

#endif
