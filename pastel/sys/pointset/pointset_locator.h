// Description: Locator of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_LOCATOR_H
#define PASTELSYS_POINTSET_LOCATOR_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/locator/location_set.h"

namespace Pastel
{

	//! Returns the member-locator of a point-set.
	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept_Member>
		> = 0
	>
	decltype(auto) pointSetLocator(
		const PointSet& pointSet)
	{
		return pointSet.pointSetLocator();
	}

	//! Returns the default locator of point-set elements.
	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept_Element>,
			// Give priority to the member-locator.
			Not<Models<PointSet, PointSet_Concept_Member>>
		> = 0
	>
	decltype(auto) pointSetLocator(const PointSet& pointSet)
	{
		using Locator = typename Default_Locator<const Set_Element<PointSet>&>::Locator;
		return Locator();
	}

}

namespace Pastel
{

	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	struct PointSet_Locator_F 
		: Identity_F<
			RemoveCvRef<decltype(pointSetLocator(std::declval<PointSet>()))>>
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

#endif
