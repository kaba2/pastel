// Description: Point-set concept
// Documentation: pointset.txt

#ifndef PASTELSYS_POINTSET_CONCEPT_H
#define PASTELSYS_POINTSET_CONCEPT_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/type_traits/or.h"

#include "pastel/sys/pointset/pointset_locator.h"

namespace Pastel
{

	struct PointSet_Concept
	: Refines<Set_Concept>
	{
		template <typename Type>
		auto requires_(
			Type&& t) -> decltype
		(
			conceptCheck(
				// A point-set is a set together with a locator. 
				Concept::models<Locator_Concept>(pointSetLocator(addConst(t)))
			)
		);
	};

}

#include "pastel/sys/pointset/pointset_dimension.h"
#include "pastel/sys/pointset/pointset_empty.h"
#include "pastel/sys/pointset/pointset_set.h"
#include "pastel/sys/pointset/pointset_n.h"
#include "pastel/sys/pointset/pointset_point_id.h"
#include "pastel/sys/pointset/pointset_point.h"
#include "pastel/sys/pointset/pointset_real.h"

#endif
