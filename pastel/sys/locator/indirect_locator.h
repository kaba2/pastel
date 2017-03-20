// Description: Indirect locator
// Documentation: locators.txt

#ifndef PASTELSYS_INDIRECT_LOCATOR_H
#define PASTELSYS_INDIRECT_LOCATOR_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/locator/transform_locator.h"

namespace Pastel
{

	template <
		typename Point,
		typename Locator>
	auto indirectLocator(
		Locator&& locator)
	{
		return transformLocator<Point>(
			std::forward<Locator>(locator),
			[](auto&& point){return *point;});
	}

}

#endif
