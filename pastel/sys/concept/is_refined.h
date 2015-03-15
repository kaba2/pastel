// Description: Check whether a concept is refined

#ifndef PASTELSYS_IS_REFINED_H
#define PASTELSYS_IS_REFINED_H

#include "pastel/sys/concept/base_concepts.h"
#include "pastel/sys/type_traits/not.h"

#include <type_traits>

namespace Pastel
{

	//! Checks whether a concept is refined.
	template <typename Concept>
	struct IsRefined
	{
		static PASTEL_CONSTEXPR bool value =
			Not<std::is_same<BaseConcepts<Concept>, Refines<>>>::value;
	};

}

#endif
