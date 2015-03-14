// Description: Check whether a concept is refined

#ifndef PASTELSYS_IS_REFINED_H
#define PASTELSYS_IS_REFINED_H

#include "pastel/sys/concept/base_concepts.h"

#include <type_traits>

namespace Pastel
{

	//! Checks whether a concept is refined.
	template <typename Concept>
	struct IsRefined
		: std::integral_constant<bool, !std::is_same<
		typename BaseConcepts<Concept>::type, Refines<>>::value>
	{};

}

#endif
