// Description: Point-id-type of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_POINT_ID_H
#define PASTELSYS_POINTSET_POINT_ID_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/function/identity_function.h"

#include <type_traits>

namespace Pastel
{

	template <typename PointSet>
	using PointSet_PointId = 
		Set_Element<PointSet_Set<PointSet>>;

	template <typename PointSet>
	using PointSet_PointId_F = 
		Identity_F<PointSet_PointId<PointSet>>;

}

#endif
