// Description: Point-set concept
// Documentation: pointset.txt

#ifndef PASTELSYS_POINTSET_CONCEPT_H
#define PASTELSYS_POINTSET_CONCEPT_H

#include "pastel/sys/input/input_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/locator/location_set.h"
#include "pastel/sys/type_traits/is_template_instance.h"
#include "pastel/sys/type_traits/or.h"

namespace Pastel
{

	struct PointSet_Concept
	{
		// A PointSet is either 
		// * an Input, or
		// * a LocationSet, which pairs an Input with a Locator.
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				RequiresSome<
					Models<Type, Input_Concept>,
					IsTemplateInstance<Type, LocationSet>
				>()
			)
		);
	};

}

#include "pastel/sys/pointset/pointset_dimension.h"
#include "pastel/sys/pointset/pointset_empty.h"
#include "pastel/sys/pointset/pointset_get.h"
#include "pastel/sys/pointset/pointset_input.h"
#include "pastel/sys/pointset/pointset_location.h"
#include "pastel/sys/pointset/pointset_locator.h"
#include "pastel/sys/pointset/pointset_point.h"
#include "pastel/sys/pointset/pointset_pop.h"
#include "pastel/sys/pointset/pointset_real.h"

#endif
