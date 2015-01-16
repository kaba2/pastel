// Description: Locator compile-time dimension

#ifndef PASTELSYS_LOCATOR_N_H
#define PASTELSYS_LOCATOR_N_H

#include "pastel/sys/locator_concept.h"

#include <type_traits>

namespace Pastel
{

	template <typename Locator>
	struct Locator_N
		: std::integral_constant<integer, Locator::N>
	{
	};

}

#endif
