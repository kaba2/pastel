// Description: Locator point-type

#ifndef PASTELSYS_LOCATOR_POINT_H
#define PASTELSYS_LOCATOR_POINT_H

#include "pastel/sys/locator/locator_concept.h"

namespace Pastel
{

	template <typename Locator>
	struct Locator_Point_F
	{
		PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);
		
		using type = typename Locator::Point;
	};

	template <typename Locator>
	using Locator_Point = 
		typename Locator_Point_F<Locator>::type;

}

#endif
