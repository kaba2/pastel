// Description: Emptiness of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_EMPTY_H
#define PASTELSYS_POINTSET_EMPTY_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/pointset/pointset_input.h"

namespace Pastel
{

	//! Returns whether a point-set is empty.
	template <typename PointSet>
	bool pointSetEmpty(const PointSet& pointSet)
	{
		PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);
		return pointSetInput(pointSet).empty();
	}

}

#endif
