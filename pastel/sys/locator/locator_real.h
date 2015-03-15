// Description: Locator coordinate-type

#ifndef PASTELSYS_LOCATOR_REAL_H
#define PASTELSYS_LOCATOR_REAL_H

#include "pastel/sys/locator/locator_concept.h"

#include <type_traits>

namespace Pastel
{

	namespace Locator_
	{

		template <typename Locator>
		struct Locator_Real_F
		{
			PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);

			using type = typename Locator::Real;
		};

	}

}

namespace Pastel
{

	template <typename... LocatorSet>
	using Locator_Real_F =
		std::common_type<
			typename Locator_::Locator_Real_F<LocatorSet>::type...
		>;

	template <typename... LocatorSet>
	using Locator_Real = 
		typename Locator_Real_F<LocatorSet...>::type;

}
	
#endif
