// Description: Array locator
// Documentation: locators.txt

#ifndef PASTELSYS_ARRAY_LOCATOR_H
#define PASTELSYS_ARRAY_LOCATOR_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/locator/default_locator.h"

#include <type_traits>
#include <array>

namespace Pastel
{

	template <
		typename Type,
		int N,
		Requires<
			Models<std::array<Type, N>, Point_Concept>
		> = 0
	>
	decltype(auto) pointLocator(std::array<Type, N>&& point)
	{
		return Default_Locator<std::array<Type, N>, Type, N>(N);
	}

	template <typename Real, int N>
	using Array_Locator = Default_Locator<std::array<Real, N>, Real, N>;

	template <typename Real_, int N_>
	decltype(auto) arrayLocator(integer n = N_)
	{
		return Array_Locator<Real_, N_>(n);
	}

}

#endif
