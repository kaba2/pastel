// Description: Point-type of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_POINT_H
#define PASTELSYS_POINTSET_POINT_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/pointset/pointset_point_id.h"
#include "pastel/sys/range/range_concept.h"
#include "pastel/sys/function/identity_function.h"
#include "pastel/sys/type_traits/remove_cvref.h"

#include <type_traits>

namespace Pastel
{

	template <PointSet_Concept PointSet>
	using PointSet_Point = RemoveCvRef<ranges::range_value_t<PointSet>>;

}

#endif
