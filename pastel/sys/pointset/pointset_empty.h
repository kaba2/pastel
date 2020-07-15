// Description: Emptiness of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_EMPTY_H
#define PASTELSYS_POINTSET_EMPTY_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/pointset/pointset_locator.h"

namespace Pastel
{

	//! Returns the dimension of a point-set.
	template <PointSet_Concept PointSet>
	requires (!Range_Concept<PointSet>)
	bool isEmptyRange(const PointSet& pointSet)
	{
		return isEmptyRange(pointSetRange(pointSet));
	}

}

#endif
