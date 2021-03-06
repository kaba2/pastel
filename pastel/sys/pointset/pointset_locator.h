// Description: Locator of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_LOCATOR_H
#define PASTELSYS_POINTSET_LOCATOR_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/locator/location_set.h"
#include "pastel/sys/range/range_concept.h"
#include "pastel/sys/locator/default_locator.h"

namespace Pastel
{

	template <typename T>
	concept HasMemberPointSetLocator__ = requires(T t) {
		{addConst(t).pointSetLocator()} -> Locator_Concept;	
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
	template <Range_Concept Range>
	requires
		(Point_Concept<Range_Value<Range>> &&
		// Give priority to the member-locator.
		!HasMemberPointSetLocator<Range>::value)
	decltype(auto) pointSetLocator(const Range& set)
	{
		using Point = Range_Value<Range>;
		using Real = Point_Real<Point>;
		static constexpr int N = Point_N<Point>::value;

		return Default_Locator<Point, Real, N>(N);
	}

}

namespace Pastel
{

	template <typename PointSet>
	using PointSet_Locator =
		RemoveCvRef<decltype(pointSetLocator(std::declval<PointSet>()))>;

}

#endif
