// Description: Set of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_SET_H
#define PASTELSYS_POINTSET_SET_H

#include "pastel/sys/pointset/pointset_concept.h"

namespace Pastel
{

	template <typename T>
	concept HasMemberPointSet__ = requires(T t) {
		addConst(t).pointSetRange();
	};

	template <typename T>
	concept HasMemberPointSet_ = 
		HasMemberPointSet__<RemoveCvRef<T>>;

	template <typename Type>
	using HasMemberPointSet = 
		std::bool_constant<HasMemberPointSet_<Type>>;

	template <HasMemberPointSet_ Type>
	decltype(auto) pointSetRange(Type&& that)
	{
		return std::forward<Type>(that).pointSetRange();
	}


	template <Range_Concept Set>
	requires 
		// Give priority to the member-locator.
		(!HasMemberPointSet<Set>::value)
	decltype(auto) pointSetRange(Set&& set)
	{
		return std::forward<Set>(set);
	}

}

namespace Pastel
{

	template <PointSet_Concept PointSet>
	using PointSet_Range = RemoveCvRef<decltype(pointSetRange(std::declval<PointSet>()))>;

}

#endif
