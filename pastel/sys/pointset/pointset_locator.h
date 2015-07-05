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

		template <typename Point_Set>
		struct PointSet_Locator_F_
			: Identity_F<
				typename Default_Locator<
					const Set_Element<Point_Set>&
				>::Locator
			>
		{};

		template <typename Point_Set, typename Locator>
		struct PointSet_Locator_F_<LocationSet<Point_Set, Locator>>
		: Identity_F<Locator>
		{};

	}

	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	struct PointSet_Locator_F
	: PointSet_::PointSet_Locator_F_<PointSet>
	{};

	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	using PointSet_Locator =
		typename PointSet_Locator_F<PointSet>::type;

}

namespace Pastel
{

	//! Returns the default locator of a point-set.
	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	decltype(auto) pointSetLocator(const PointSet& pointSet)
	{
		return PointSet_Locator<PointSet>();
	}

	//! Retrieves the locator of a location-set.
	template <typename Set, typename Locator>
	const Locator& pointSetLocator(
		const LocationSet<Set, Locator>& pointSet)
	{
		return pointSet.locator();
	}

}

#endif
