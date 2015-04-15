#ifndef PASTELGEOMETRY_NEARESTSET_POINTSET_H
#define PASTELGEOMETRY_NEARESTSET_POINTSET_H

#include "pastel/geometry/nearestset/nearestset_concept.h"

namespace Pastel
{

	template <typename NearestSet>
	struct NearestSet_PointSet_F
	{
		PASTEL_CONCEPT_CHECK(NearestSet, NearestSet_Concept);

		using type = typename NearestSet::PointSet;
	};

	template <typename NearestSet>
	using NearestSet_PointSet = 
		typename NearestSet_PointSet_F<NearestSet>::type;

}

#endif
