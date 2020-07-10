// Description: Emptiness of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_EMPTY_H
#define PASTELSYS_POINTSET_EMPTY_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/pointset/pointset_locator.h"

namespace Pastel
{

	//! Returns the dimension of a point-set.
	template <PointSet_Concept_ PointSet>
	requires (!Set_Concept_<PointSet>)
	bool emptySet(const PointSet& pointSet)
	{
		return emptySet(pointSetSet(pointSet));
	}

}

#endif
