// Description: Locator coordinate-type

#ifndef PASTELSYS_LOCATOR_REAL_H
#define PASTELSYS_LOCATOR_REAL_H

#include "pastel/sys/locator_concept.h"

#include <type_traits>

namespace Pastel
{

	namespace Locator_
	{

		template <typename... LocatorSet>
		struct Locator_Real;

		template <typename Locator, typename... LocatorSet>
		struct Locator_Real<Locator, LocatorSet...>
		{
			using type = std::common_type_t<
				typename Locator_Real<Locator>::type, 
				typename Locator_Real<LocatorSet...>::type>;
		};

		template <typename Locator>
		struct Locator_Real<Locator>
		{
			using type = typename Locator::Real;
		};

	}

	template <typename... LocatorSet>
	using Locator_Real = typename Locator_::Locator_Real<LocatorSet...>::type;

}
	
#endif
