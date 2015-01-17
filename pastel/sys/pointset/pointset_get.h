// Description: Element of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_GET_H
#define PASTELSYS_POINTSET_GET_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/locator/location_set.h"
#include "pastel/sys/locator/location.h"

namespace Pastel
{

	template <typename Point_Input>
	auto pointSetGet(const Point_Input& pointSet)
	-> decltype(pointSet.get())
	{
		return pointSet.get();
	}

	template <
		typename Point_Input,
		typename Locator>
	auto pointSetGet(const LocationSet<Point_Input, Locator>& pointSet)
	-> decltype(location(pointSet.pointInput().get(), pointSet.locator()))
	{
		return location(pointSet.pointInput().get(), pointSet.locator());
	}

}

#endif
