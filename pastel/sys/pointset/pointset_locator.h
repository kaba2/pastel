// Description: Locator of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_LOCATOR_H
#define PASTELSYS_POINTSET_LOCATOR_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/locator/location_set.h"
#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/locator/default_locator.h"

namespace Pastel
{

	template <typename T>
	concept HasMemberPointSetLocator__ = requires(T t) {
		{addConst(t).pointSetLocator()} -> Locator_Concept_;	
	};

	template <typename T>
	concept HasMemberPointSetLocator_ = 
		HasMemberPointSetLocator__<RemoveCvRef<T>>;

	template <typename Type>
	using HasMemberPointSetLocator = 
		std::bool_constant<HasMemberPointSetLocator_<Type>>;
	
}

namespace Pastel
{

	//! Returns the member-locator of a point-set.
	template <HasMemberPointSetLocator_ Type>
	decltype(auto) pointSetLocator(Type&& that)
	{
		return std::forward<Type>(that).pointSetLocator();
	}

	//! Returns the default locator of point-set elements.
	template <Set_Concept_ Set>
	requires
		(Point_Concept_<Set_Element<Set>> &&
		// Give priority to the member-locator.
		!HasMemberPointSetLocator<Set>::value)
	decltype(auto) pointSetLocator(const Set& set)
	{
		using Point = Set_Element<Set>;
		using Real = Point_Real<Point>;
		static constexpr int N = Point_N<Point>::value;

		return Default_Locator<Point, Real, N>(N);
	}

}

namespace Pastel
{

	template <typename PointSet>
	struct PointSet_Locator_F 
		: Identity_F<
			RemoveCvRef<decltype(pointSetLocator(std::declval<PointSet>()))>>
	{};

	template <typename PointSet>
	using PointSet_Locator =
		typename PointSet_Locator_F<PointSet>::type;

}

#endif
