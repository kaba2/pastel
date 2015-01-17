// Description: Locator point-type

#ifndef PASTELSYS_LOCATOR_POINT_H
#define PASTELSYS_LOCATOR_POINT_H

#include "pastel/sys/locator/locator_concept.h"

namespace Pastel
{

	namespace Locator_
	{

		template <typename Locator>
		struct Locator_Point
		{
			using type = typename Locator::Point;
		};

	}

	template <typename Locator>
	using Locator_Point = typename Locator_::Locator_Point<Locator>::type;

}

#endif
