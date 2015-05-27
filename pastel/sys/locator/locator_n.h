// Description: Locator compile-time dimension

#ifndef PASTELSYS_LOCATOR_N_H
#define PASTELSYS_LOCATOR_N_H

#include "pastel/sys/locator/locator_concept.h"

#include <type_traits>

namespace Pastel
{

	template <typename Locator>
	struct Locator_N
	{
		PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);
		
		static constexpr integer value = 
			Locator::N;
	};



}

#endif
