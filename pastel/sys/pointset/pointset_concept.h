// Description: Point-set concept
// Documentation: pointset.txt

#ifndef PASTELSYS_POINTSET_CONCEPT_H
#define PASTELSYS_POINTSET_CONCEPT_H

#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/type_traits/or.h"

namespace Pastel
{

	struct PointSet_Concept
	{
		// A PointSet is a Set of elements together with an 
		// associated locator for those elements.
		// The locator is searched for in the following order:
		// 1) class member function locator(),
		// 2) the locator of a set-element.
		template <typename Type>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::models<Set_Concept>(pointSetSet(addConst(t))),
				Concept::models<Locator_Concept>(pointSetLocator(addConst(t)))
			)
		);
	};

}

#include "pastel/sys/pointset/pointset_dimension.h"
#include "pastel/sys/pointset/pointset_empty.h"
#include "pastel/sys/pointset/pointset_set.h"
#include "pastel/sys/pointset/pointset_locator.h"
#include "pastel/sys/pointset/pointset_n.h"
#include "pastel/sys/pointset/pointset_point_id.h"
#include "pastel/sys/pointset/pointset_point.h"
#include "pastel/sys/pointset/pointset_real.h"

#endif
