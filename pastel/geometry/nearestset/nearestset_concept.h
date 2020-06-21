// Description: Nearest-neighbor search-structure

#ifndef PASTELGEOMETRY_NEAREST_SET_CONCEPT_H
#define PASTELGEOMETRY_NEAREST_SET_CONCEPT_H

#include "pastel/math/norm/norm_concept.h"
#include "pastel/math/distance/distance_concept.h"
#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/output/output_concept.h"

namespace Pastel
{

	struct NearestSet_Concept
	: Refines<PointSet_Concept>
	{
		template <
			typename Type,
			typename Point = PointSet_Point<Type>
		>
		auto requires_(Type&& nearestSet) -> decltype
		(
			conceptCheck(
				addConst(nearestSet).asPoint(std::declval<Point>()),
				(addConst(nearestSet).findNearbyPointsets(
					// Search-point
					Point_Archetype(),
					// Norm to use
					Norm_Archetype(),
					// Culling distance
					Distance_Archetype(),
					Output_Archetype()), 0)
			)
		);
	};

	template <typename T>
	concept NearestSet_Concept_ = requires(T t) {
		addConst(t).asPoint(std::declval<PointSet_Point<T>>());
		addConst(t).findNearbyPointsets(
			// Search-point
			Point_Archetype(),
			// Norm to use
			Norm_Archetype(),
			// Culling distance
			Distance_Archetype(),
			Output_Archetype());
	};

	template <typename Type>
	using NearestSet_Point = PointSet_Point<Type>;

}

#endif
