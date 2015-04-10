// Description: Nearest-neighbor search-structure

#ifndef PASTELGEOMETRY_NEAREST_SET_CONCEPT_H
#define PASTELGEOMETRY_NEAREST_SET_CONCEPT_H

#include "pastel/sys/point/point_concept.h"

namespace Pastel
{

	struct NearestSet_Concept
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::holds<
					Models<typename Type::Point, Point_Concept>
				>(),
				Concept::convertsTo<typename Type::Point>(
					t.searchNearest(
						//! The point for which to search nearest-neighbors.
						std::declval<typename Type::Point>(),
						//! The output to which to report the results.
						[](typename Type::Point) {},
						//! Optional arguments
						[](auto& optional)
						{
							//! Maximum distance over which to search.
							optional.maxRadius = 1;
							//! The number of neighbors to search.
							optional.k = 1;
						}
					)
				)
			)
		);
	};

}

#endif
