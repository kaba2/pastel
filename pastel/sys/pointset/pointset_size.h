// Description: Size of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_SIZE_H
#define PASTELSYS_POINTSET_SIZE_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/pointset/pointset_input.h"

namespace Pastel
{

	//! Returns the size of a point-set.
	template <typename PointSet>
	integer pointSetSize(
		const PointSet& pointSet)
	{
		PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);
		return pointSetInput(pointSet).n();
	}

}

#endif
