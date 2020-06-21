#ifndef PASTELSYS_SET_INDEX_H
#define PASTELSYS_SET_INDEX_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/type_traits/remove_cvref.h"
#include "pastel/sys/function/identity_function.h"

namespace Pastel
{

	template <Set_Concept_ Set>
	using Set_Index = 
		typename ranges::iterator_t<Set>;
	
	template <Set_Concept_ Set>
	using Set_Index_F = 
		Identity_F<Set_Index<Set>>;

}

#endif
