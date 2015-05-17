// Description: Locator coordinate-type

#ifndef PASTELSYS_LOCATOR_REAL_H
#define PASTELSYS_LOCATOR_REAL_H

#include "pastel/sys/locator/locator_concept.h"

#include <type_traits>

namespace Pastel
{

	template <typename Locator>
	using Locator_Real_ =
		typename Locator::Real;

	template <typename... LocatorSet>
	using Locator_Real_F =
		std::common_type<
			Locator_Real_<LocatorSet>...
		>;

	template <typename... LocatorSet>
	using Locator_Real = 
		typename Locator_Real_F<LocatorSet...>::type;

}
	
#endif
