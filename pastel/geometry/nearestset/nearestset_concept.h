// Description: Nearest-neighbor search-structure

#ifndef PASTELGEOMETRY_NEAREST_SET_CONCEPT_H
#define PASTELGEOMETRY_NEAREST_SET_CONCEPT_H

#include "pastel/sys/pointset/pointset_concept.h"

#include "pastel/math/normbijection/normbijection_concept.h"
#include "pastel/sys/function/function_concept.h"

#include "pastel/geometry/nearestset/nearestset_pointset.h"

namespace Pastel
{

	struct NearestSet_Concept
	{
		template <
			typename Type,
			typename PointSet = NearestSet_PointSet<Type>,
			typename Point = PointSet_Point<PointSet>,
			typename Real = PointSet_Real<PointSet>
		>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Returns the underlying point-set.
				Concept::isModelOf<PointSet_Concept>(
					addConst(t).pointSet())//,
				//! Searches for nearest neighbors for a point.
				//Concept::convertsTo<std::pair<Real, Point>>
				//(
				#if 0
				searchNearest(
						// A nearest-set.
						addConst(t),
						//! The point for which to search nearest-neighbors.
						std::declval<Point>()//,
						//! The output to which to report the results.
						/*!
						The points will be reported as report(distance2, point).
						*/
						/*
						PASTEL_TAG(report), Function_Archetype<void, Real, Point>(),
						//! An indicator to decide whether to consider a given point.
						PASTEL_TAG(accept), Function_Archetype<bool, Point>(),
						//! The norm to use.
						PASTEL_TAG(normBijection), NormBijection_Archetype(),
						//! The number of nearest-neigbhors to search for.
						PASTEL_TAG(kNearest), (integer)1,
						//! The maximum distance to allow for a nearest-neighbors.
						PASTEL_TAG(maxDistance2), (Real)Infinity()*/
				//	)
				)
				#endif
			)
		);
	};

}

#include "pastel/geometry/nearestset/nearestset_point_id.h"
#include "pastel/geometry/nearestset/nearestset_real.h"
#include "pastel/geometry/nearestset/nearestset_n.h"

#endif
