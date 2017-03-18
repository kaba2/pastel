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
	: Refines<Set_Concept>
	{
		template <
			typename Type,
			typename Element = Set_Element<Type>>
		auto requires_(
			Type&& t,
			Element&& element = std::declval<Element>()) -> decltype
		(
			conceptCheck(
				// A PointSet is a Set whose elements are Points.
				Concept::models<Point_Concept>(addConst(element)),
				// In addition, a PointSet can be decomposed into
				// a set of point-ids and a locator.
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
