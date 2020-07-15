// Description: Locator coordinate-type

#ifndef PASTELSYS_LOCATOR_REAL_H
#define PASTELSYS_LOCATOR_REAL_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/function/identity_function.h"
#include "pastel/sys/type_traits/remove_cvref.h"
#include "pastel/sys/locator/locator_point.h"

namespace Pastel
{

	template <typename... LocatorSet>
	using Locator_Real = 
		std::common_type_t<
			typename LocatorSet::Real...
		>;

}
	
#endif
