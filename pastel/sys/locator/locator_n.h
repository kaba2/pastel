// Description: Locator compile-time dimension

#ifndef PASTELSYS_LOCATOR_N_H
#define PASTELSYS_LOCATOR_N_H

#include "pastel/sys/locator/locator_concept.h"

#include <type_traits>

namespace Pastel
{

	template <Locator_Concept_ Locator>
	struct Locator_N
	{
		static constexpr integer value = RemoveCvRef<Locator>::N;
	};

}

#endif
