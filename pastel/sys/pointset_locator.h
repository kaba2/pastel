// Description: Locator of a point-set

#ifndef PASTELSYS_POINTSET_LOCATOR_H
#define PASTELSYS_POINTSET_LOCATOR_H

#include "pastel/sys/pointset_concept.h"
#include "pastel/sys/point_concept.h"
#include "pastel/sys/location_set.h"

namespace Pastel
{

	namespace PointSet_
	{

		template <typename PointSet>
		struct PointSet_Locator
		{
			using type = 
				decltype(locator(std::declval<PointSet>().get()));
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
	/*!
	The default locator is the default locator
	of the first point of the point-set.
	*/
	template <typename Point_Input>
	auto pointSetLocator(const Point_Input& pointSet)
		-> decltype(locator(pointSet.get()))
	{
		// FIX: Replace with decltype(auto) when available.
		return locator(pointSet.get());
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
