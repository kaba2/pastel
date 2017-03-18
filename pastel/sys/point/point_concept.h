// Description: Point concept

#ifndef PASTELSYS_POINT_CONCEPT_H
#define PASTELSYS_POINT_CONCEPT_H

#include "pastel/sys/locator/location.h"
#include "pastel/sys/real/real_ring_concept.h"

namespace Pastel
{

	struct Point_Concept
	{
		template <typename Type>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::models<Real_Ring_Concept>(pointAxis(addConst(t), (integer)0)),
				Concept::convertsTo<integer>(dimension(addConst(t)))
				// ,
				// Concept::models<Locator>(pointLocator(addConst(t)))
				// pointPointId(addConst(t))
			)
		);
	};

}

#include "pastel/sys/point/point_archetype.h"
#include "pastel/sys/point/point_locator.h"
#include "pastel/sys/point/point_axis.h"
#include "pastel/sys/point/point_dimension.h"
#include "pastel/sys/point/point_point_id.h"
#include "pastel/sys/point/point_real.h"

#include "pastel/sys/point/number_point.h"

#endif
