// Description: Array locator
// Documentation: locators.txt

#ifndef PASTELSYS_ARRAY_LOCATOR_H
#define PASTELSYS_ARRAY_LOCATOR_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/ensure.h"

#include <type_traits>
#include <array>

namespace Pastel
{

	template <typename Real, integer N>
	using Array_Locator = Default_Locator<std::array<Real, N>, Real, N>;

	template <typename Real_, integer N_>
	decltype(auto) arrayLocator(integer n = N_)
	{
		return Array_Locator<Real_, N_>(n);
	}

}

#endif
