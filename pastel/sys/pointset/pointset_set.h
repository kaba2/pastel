// Description: Set of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_SET_H
#define PASTELSYS_POINTSET_SET_H

#include "pastel/sys/pointset/pointset_concept.h"

namespace Pastel
{

	namespace PointSet_
	{

		//! Returns the point-set itself.
		template <typename Point_Set>
		decltype(auto) pointSetSet(
			Point_Set&& that)
		{
			return std::forward<Point_Set>(that);
		}

		//! Returns the point-set of a location-set.
		template <typename Point_Set, typename Locator>
		decltype(auto) pointSetSet(
			const LocationSet<Point_Set, Locator>& pointSet)
		{
			return pointSet.pointSet();
		}

		template <typename Point_Set, typename Locator>
		decltype(auto) pointSetSet(
			LocationSet<Point_Set, Locator>& pointSet)
		{
			return pointSet.pointSet();
		}

	}

	//! Returns the point-input.
	template <
		typename PointSet,
		Requires<Models<PointSet, PointSet_Concept>> = 0
	>
	decltype(auto) pointSetSet(PointSet&& pointSet)
	{
		return PointSet_::pointSetSet(std::forward<PointSet>(pointSet));
	}

}

namespace Pastel
{

	template <typename PointSet>
	struct PointSet_Set_F
	: Identity_F<PointSet>
	{};

	template <
		typename Set,
		typename Locator>
	struct PointSet_Set_F<LocationSet<Set, Locator>>
	: Identity_F<Set>
	{};

	template <typename PointSet>
	using PointSet_Set = 
		typename PointSet_Set_F<PointSet>::type;

}

#endif
