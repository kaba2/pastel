// Description: Locator point-type

#ifndef PASTELSYS_LOCATOR_POINT_H
#define PASTELSYS_LOCATOR_POINT_H

#include "pastel/sys/locator/locator_concept.h"

namespace Pastel
{

	template <typename Locator>
	using Locator_Point = 
		typename Locator::Point;

	template <typename Locator>
	using Locator_Point_F = 
		Identity_F<Locator_Point<Locator>>;

}

#endif
