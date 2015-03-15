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
		struct PointSet_Locator_F
		{
			using Point = 
				decltype(std::declval<Point_Input>().get());
			using type = 
				typename Default_Locator<const Point&>::Locator;
		};

		template <typename Point_Input, typename Locator>
		struct PointSet_Locator_F<LocationSet<Point_Input, Locator>>
		{
			using type = Locator;
		};

	}

}

namespace Pastel
{

	template <typename PointSet>
	struct PointSet_Locator_F
	{
		PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);

		using type = 
			typename PointSet_::PointSet_Locator_F<PointSet>::type;
	};

	template <typename PointSet>
	using PointSet_Locator =
		typename PointSet_Locator_F<PointSet>::type;

}

namespace Pastel
{

	//! Returns the default locator of a point-set.
	template <typename Point_Input>
	decltype(auto) pointSetLocator(const Point_Input& pointSet)
	{
		PASTEL_CONCEPT_CHECK(Point_Input, Input_Concept);
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
