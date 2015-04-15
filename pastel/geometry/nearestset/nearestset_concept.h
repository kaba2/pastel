// Description: Nearest-neighbor search-structure

#ifndef PASTELGEOMETRY_NEAREST_SET_CONCEPT_H
#define PASTELGEOMETRY_NEAREST_SET_CONCEPT_H

#include "pastel/sys/pointset/pointset_concept.h"

#include "pastel/math/normbijection/euclidean_normbijection.h"
#include "pastel/sys/output/output_concept.h"

namespace Pastel
{

	struct NearestSet_Concept
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::exists<typename Type::PointSet>(),
				Concept::models<typename Type::PointSet, PointSet_Concept>(),
				Concept::convertsTo<typename Type::PointSet>(
					addConst(t).pointSet()
				)
				#if 0
				,
				Concept::convertsTo<
					std::pair<
						PointSet_Real<typename Type::PointSet>, 
						PointSet_Point<typename Type::PointSet>
					>
				>(
					searchNearest(
						// A nearest-set.
						t,
						//! The point for which to search nearest-neighbors.
						std::declval<PointSet_Point<typename Type::PointSet>>(),
						Output_Archetype()/*,
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
						Euclidean_NormBijection<typename Type::Real>(),
						//! Optional arguments
						[](auto& optional)
						{
							//! Maximum distance over which to search.
							optional.maxDistance = 1;
							//! The number of neighbors to search.
							optional.k = 1;
						}*/
					)
				)
				#endif
			)
		);
	};

}

#include "pastel/geometry/nearestset/nearestset_pointset.h"
#include "pastel/geometry/nearestset/nearestset_point.h"
#include "pastel/geometry/nearestset/nearestset_real.h"

#endif
