// Description: Norm concept
// Documentation: norms.txt

#ifndef PASTELMATH_NORM_CONCEPT_H
#define PASTELMATH_NORM_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/distance/distance_concept.h"

namespace Pastel
{

	template <typename T>
	concept Norm_Concept_ = requires(T t) {
		//! Returns a zero distance.
		{addConst(t)()} -> Distance_Concept;
	};

	template <typename T>
	concept Norm_Concept = 
		Norm_Concept_<RemoveCvRef<T>>;

}

#include "pastel/math/norm/norm_archetype.h"

#endif

