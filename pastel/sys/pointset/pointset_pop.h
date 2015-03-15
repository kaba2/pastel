// Description: Next point of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_POP_H
#define PASTELSYS_POINTSET_POP_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/pointset/pointset_input.h"

namespace Pastel
{

	template <typename PointSet>
	auto pointSetPop(PointSet& pointSet)
	{
		PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);
		pointSetInput(pointSet).pop();
	}

}

#endif
