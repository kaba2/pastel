// Description: Set concept

#ifndef PASTELSYS_SET_CONCEPT_H
#define PASTELSYS_SET_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/type_traits/remove_cvref.h"

#include <range/v3/range/concepts.hpp>

namespace Pastel
{

	//! Set
	/*!
	To be precise, the set-concept represents a multi-set;
	elements can occur multiple times. Here we use the term 
	'set' a bit loosely, for brevity.
	*/
	template <typename T>
	concept Set_Concept_ = requires(T t) {
		t;
		//Concept::holds<ranges::concepts::models<ranges::concepts::Range, Type>>()
	};

	template <typename T>
	concept Set_Concept = 
		Set_Concept_<RemoveCvRef<T>>;

	template <typename T>
	concept Range_Concept = 
		Set_Concept<T>;
	
}

#include "pastel/sys/set/set_element.h"
#include "pastel/sys/set/set_empty.h"
#include "pastel/sys/set/set_index.h"
#include "pastel/sys/set/set_for_each.h"
#include "pastel/sys/set/set_size.h"

#endif
