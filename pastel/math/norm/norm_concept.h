// Description: Norm concept
// Documentation: norms.txt

#ifndef PASTELMATH_NORM_CONCEPT_H
#define PASTELMATH_NORM_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/distance/distance_concept.h"

namespace Pastel
{

	struct Norm_Concept
	{
		template <typename Type>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				//! Returns a zero distance.
				Concept::isModelOf<Distance_Concept>(
					addConst(t)()
				)
			)
		);
	};

}

#include "pastel/math/norm/norm_archetype.h"

#endif

