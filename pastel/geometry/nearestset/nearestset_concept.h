// Description: Nearest-neighbor search-structure

#ifndef PASTELGEOMETRY_NEAREST_SET_CONCEPT_H
#define PASTELGEOMETRY_NEAREST_SET_CONCEPT_H

#include "pastel/sys/pointset/pointset_concept.h"

#include "pastel/math/normbijection/normbijection_concept.h"
#include "pastel/sys/function/function_concept.h"

namespace Pastel
{

	struct NearestSet_Concept
	: Refines<PointSet_Concept>
	{
		template <
			typename Type,
			typename PointId = PointSet_PointId<Type>,
			typename Real = PointSet_Real<Type>
		>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				//! Searches for nearest neighbors for a point.
				Concept::convertsTo<std::pair<Real, PointId>>
				(
					searchNearest(
						// A nearest-set.
						addConst(t),
						//! The point for which to search nearest-neighbors.
						/*!
						This should read Point_Archetype; the search-point does
						not have to have Point type. However, for some reason that
						triggers compiler errors.
						*/
						std::declval<Point_Archetype>(),
						//! The output to which to report the results.
						/*!
						The points will be reported as report(distance2, point).
						*/
						PASTEL_TAG(report), Function_Archetype<void, Real, PointId>(),
						//! An indicator to decide whether to consider a given point.
						PASTEL_TAG(accept), Function_Archetype<bool, PointId>(),
						//! The norm to use.
						PASTEL_TAG(normBijection), NormBijection_Archetype(),
						//! The number of nearest-neigbhors to search for.
						PASTEL_TAG(kNearest), (integer)1,
						//! The maximum distance to allow for a nearest-neighbors.
						PASTEL_TAG(maxDistance2), (Real)Infinity()
					)
				)
			)
		);
	};

}

#include "pastel/geometry/nearestset/nearestset_n.h"

#endif
