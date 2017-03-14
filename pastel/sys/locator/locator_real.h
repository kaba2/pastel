// Description: Locator coordinate-type

#ifndef PASTELSYS_LOCATOR_REAL_H
#define PASTELSYS_LOCATOR_REAL_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/function/identity_function.h"
#include "pastel/sys/type_traits/remove_cvref.h"

namespace Pastel
{

	template <
		typename Locator,
		Requires<
			Models<Locator, Locator_Concept>
		> = 0
	>
	using Locator_Real_ =
		typename RemoveCvRef<Locator>::Real;

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
