// Description: Point-type of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_POINT_H
#define PASTELSYS_POINTSET_POINT_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/pointset/pointset_point_id.h"
#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/locator/location_set.h"
#include "pastel/sys/function/identity_function.h"
#include "pastel/sys/type_traits/remove_cvref.h"

#include <type_traits>

namespace Pastel
{

	namespace PointSet_Point_
	{

		template <typename PointSet>
		struct PointSet_Point_F_
		{
			using type = PointSet_PointId<PointSet>;
		};

		template <
			typename Set, 
			typename Locator,
			typename Base>
		struct PointSet_Point_F_<LocationSet<Set, Locator, Base>>
		{
			using type = Location<Set_Element<Set>, Locator>;
		};

	}

	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	using PointSet_Point = 
		typename PointSet_Point_::PointSet_Point_F_<
			RemoveCvRef<PointSet>
		>::type;

	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	using PointSet_Point_F = 
		Identity_F<PointSet_Point<PointSet>>;

}

#endif
