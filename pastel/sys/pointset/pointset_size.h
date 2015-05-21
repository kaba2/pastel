// Description: Size of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_SIZE_H
#define PASTELSYS_POINTSET_SIZE_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/pointset/pointset_input.h"

namespace Pastel
{

	//! Returns the size of a point-set.
	template <
		typename PointSet,
		Requires<Models<PointSet, PointSet_Concept>> = 0
	>
	integer pointSetSize(
		const PointSet& pointSet)
	{
		return pointSetInput(pointSet).n();
	}

}

#endif
