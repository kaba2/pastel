// Description: Locator point-type

#ifndef PASTELSYS_LOCATOR_POINT_H
#define PASTELSYS_LOCATOR_POINT_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/function/identity_function.h"
#include "pastel/sys/type_traits/remove_cvref.h"

namespace Pastel
{

	template <typename Locator>
	using Locator_Point = 
		typename RemoveCvRef<Locator>::Point;

	template <typename Locator>
	using Locator_Point_F = 
		Identity_F<Locator_Point<Locator>>;

}

#endif
