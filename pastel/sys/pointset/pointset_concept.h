// Description: Point-set concept
// Documentation: pointset.txt

#ifndef PASTELSYS_POINTSET_CONCEPT_H
#define PASTELSYS_POINTSET_CONCEPT_H

#include "pastel/sys/set/range_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/type_traits/or.h"

#include "pastel/sys/pointset/pointset_locator.h"

namespace Pastel
{

	template <typename T>
	concept PointSet_Concept_ =
		Range_Concept<T> && 
		requires(T t) {
		// A point-set is a set together with a locator. 
		pointSetLocator(addConst(t));
		{removeReference(pointSetLocator(addConst(t)))} -> Locator_Concept;
	};

	template <typename T>
	concept PointSet_Concept =
		PointSet_Concept_<RemoveCvRef<T>>;

}

#include "pastel/sys/pointset/pointset_dimension.h"
#include "pastel/sys/pointset/pointset_empty.h"
#include "pastel/sys/pointset/pointset_range.h"
#include "pastel/sys/pointset/pointset_n.h"
#include "pastel/sys/pointset/pointset_point_id.h"
#include "pastel/sys/pointset/pointset_point.h"
#include "pastel/sys/pointset/pointset_real.h"

#endif
