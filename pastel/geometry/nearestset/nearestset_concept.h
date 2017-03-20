// Description: Nearest-neighbor search-structure

#ifndef PASTELGEOMETRY_NEAREST_SET_CONCEPT_H
#define PASTELGEOMETRY_NEAREST_SET_CONCEPT_H

#include "pastel/math/normbijection/normbijection_concept.h"
#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/pointset/pointset_concept.h"

namespace Pastel
{

	struct NearestSet_Concept
	: Refines<PointSet_Concept>
	{
		template <typename Type>
		auto requires_(Type&& nearestSet) -> decltype
		(
			conceptCheck(
				addConst(nearestSet).asPoint(*ranges::begin(addConst(nearestSet))),
				// Concept::models<Set_Concept>(
					addConst(nearestSet).nearbyPointSetSet(
						// Search-point
						Point_Archetype(),
						// Norm to use
						NormBijection_Archetype(),
						// Reference to culling distance
						(const real&)std::declval<real>())
				// )
			)
		);
	};

	template <typename Type>
	using NearestSet_Point = PointSet_Point<Type>;
		//decltype(*ranges::begin(std::declval<Type>()));

}

#endif
