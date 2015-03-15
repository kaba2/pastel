// Description: Element of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_GET_H
#define PASTELSYS_POINTSET_GET_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/locator/location_set.h"
#include "pastel/sys/locator/location.h"

namespace Pastel
{

	namespace PointSet_
	{

		template <typename Point_Input>
		decltype(auto) pointSetGet(
			const Point_Input& pointSet)
		{
			return pointSet.get();
		}

		template <
			typename Point_Input,
			typename Locator>
		decltype(auto) pointSetGet(
			const LocationSet<Point_Input, Locator>& pointSet)
		{
			return location(
				pointSet.pointInput().get(), 
				pointSet.locator());
		}

	}

	template <typename PointSet>
	decltype(auto) pointSetGet(const PointSet& pointSet)
	{
		PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);
		return PointSet_::pointSetGet(pointSet);
	}

}

#endif
