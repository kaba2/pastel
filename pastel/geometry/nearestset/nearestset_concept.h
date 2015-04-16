// Description: Nearest-neighbor search-structure

#ifndef PASTELGEOMETRY_NEAREST_SET_CONCEPT_H
#define PASTELGEOMETRY_NEAREST_SET_CONCEPT_H

#include "pastel/sys/pointset/pointset_concept.h"

#include "pastel/math/normbijection/normbijection_concept.h"
#include "pastel/sys/output/output_concept.h"

namespace Pastel
{

	struct NearestSet_Concept
	{
		template <
			typename Type,
			typename PointSet = typename Type::PointSet>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::exists<PointSet>(),
				Concept::models<PointSet, PointSet_Concept>(),
				Concept::convertsTo<PointSet>(addConst(t).pointSet())
				/*
				,				
				(
					std::pair<
						PointSet_Real<PointSet), 
						PointSet_Point<PointSet>
					>
				)
				(
					searchNearest(
						// A nearest-set.
						addConst(t),
						//! The point for which to search nearest-neighbors.
						std::declval<PointSet_Point<PointSet>>()
						/*,
						Output_Archetype(),
						//! The output to which to report the results.
						[](typename Type::Real distance,
						   typename Type::Point point) 
						{
						},
						//! An indicator which decides whether to consider a given point.
						[](typename Type::Point) 
						{
							return true;
						},
						NormBijection_Archetype(),
						//! Optional arguments
						[](auto& optional)
						{
							//! Maximum distance over which to search.
							optional.maxDistance = 1;
							//! The number of neighbors to search.
							optional.k = 1;
						}
					)
				)*/
			)
		);
	};

}

#include "pastel/geometry/nearestset/nearestset_pointset.h"
#include "pastel/geometry/nearestset/nearestset_point.h"
#include "pastel/geometry/nearestset/nearestset_real.h"

#endif
