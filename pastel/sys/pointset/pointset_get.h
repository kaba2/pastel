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
			// Propagate the locator of the point-set.
			// Note that we need to propagate the locator
			// even when a point has a default-locator.
			// This is because a locator can contain run-time
			// data.
			return location(
				pointSet.pointInput().get(), 
				pointSet.locator());
		}

	}

	template <
		typename PointSet,
		typename Constraint =
			Requires<
				Models<PointSet, PointSet_Concept>
			>
	>
	decltype(auto) pointSetGet(const PointSet& pointSet)
	{
		return PointSet_::pointSetGet(pointSet);
	}

}

#endif
