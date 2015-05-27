// Description: Compile-time dimension of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_N_H
#define PASTELSYS_POINTSET_N_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/pointset/pointset_locator.h"

namespace Pastel
{

	template <typename PointSet>
	struct PointSet_N
	{
		PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);

		static constexpr integer value =
			Locator_N<PointSet_Locator<PointSet>>::value;
	};

}

#endif
