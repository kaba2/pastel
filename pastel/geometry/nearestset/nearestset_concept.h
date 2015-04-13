// Description: Nearest-neighbor search-structure

#ifndef PASTELGEOMETRY_NEAREST_SET_CONCEPT_H
#define PASTELGEOMETRY_NEAREST_SET_CONCEPT_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/real/real_concept.h"
#include "pastel/math/normbijection/euclidean_normbijection.h"
#include "pastel/sys/output/null_output.h"

namespace Pastel
{

	struct NearestSet_Concept
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::exists<typename Type::Real>(),
				Concept::exists<typename Type::Point>(),
				Concept::holds<
					Models<typename Type::Real, Real_Concept>/*,
					Models<typename Type::Point, Point_Concept>*/
				>(),
				Concept::convertsTo<
					std::pair<
						typename Type::Real, 
						typename Type::Point
					>
				>(
					searchNearest(
						// A nearest-set.
						t,
						//! The point for which to search nearest-neighbors.
						std::declval<typename Type::Point>(),
						Null_Output()
						/*,
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
			)
		);
	};

}

namespace Pastel
{

	template <typename NearestSet>
	struct NearestSet_Point_F
	{
		using type = typename NearestSet::Point;
	};

	template <typename NearestSet>
	using NearestSet_Point = 
		typename NearestSet_Point_F<NearestSet>::type;

}

namespace Pastel
{

	template <typename NearestSet>
	struct NearestSet_Real_F
	{
		using type = typename NearestSet::Real;
	};

	template <typename NearestSet>
	using NearestSet_Real = 
		typename NearestSet_Real_F<NearestSet>::type;

}

#endif
