// Description: Component-type of a nearest-set point
// Documentation: nearestset.txt

#ifndef PASTELGEOMETRY_NEARESTSET_REAL_H
#define PASTELGEOMETRY_NEARESTSET_REAL_H

#include "pastel/geometry/nearestset/nearestset_pointset.h"

namespace Pastel
{

	template <typename NearestSet>
	struct NearestSet_Real_F
	{
		PASTEL_CONCEPT_CHECK(NearestSet, NearestSet_Concept);

		using type = 
			PointSet_Real<NearestSet_PointSet<NearestSet>>;
	};

	template <typename NearestSet>
	using NearestSet_Real = 
		typename NearestSet_Real_F<NearestSet>::type;

}

#endif
