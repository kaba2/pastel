// Description: Locator of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_LOCATOR_H
#define PASTELSYS_POINTSET_LOCATOR_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/locator/location_set.h"

namespace Pastel
{

	namespace PointSet_
	{

		template <typename Point_Input>
		struct PointSet_Locator
		{
			using Point = 
				decltype(std::declval<Point_Input>().get());
			using type = 
				typename Default_Locator<const Point&>::Locator;
		};

		template <typename Point_Input, typename Locator>
		struct PointSet_Locator<LocationSet<Point_Input, Locator>>
		{
			using type = Locator;
		};

	}

	template <typename PointSet>
	using PointSet_Locator = 
		typename PointSet_::PointSet_Locator<PointSet>::type;

	//! Returns the default locator of a point-set.
	template <typename Point_Input>
	PointSet_Locator<Point_Input> 
		pointSetLocator(const Point_Input& pointSet)
	{
		return PointSet_Locator<Point_Input>();
	}

	//! Retrieves the locator of a location-set.
	template <typename Point_Input, typename Locator>
	const Locator& pointSetLocator(
		const LocationSet<Point_Input, Locator>& pointSet)
	{
		return pointSet.locator();
	}

}

#endif
