// Description: Emptiness of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_EMPTY_H
#define PASTELSYS_POINTSET_EMPTY_H

#include "pastel/sys/pointset_concept.h"
#include "pastel/sys/pointset_input.h"

namespace Pastel
{

	template <typename PointSet>
	bool pointSetEmpty(const PointSet& pointSet)
	{
		return pointSetInput(pointSet).empty();
	}

}

#endif
