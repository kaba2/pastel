// Description: Dimension of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_DIMENSION_H
#define PASTELSYS_POINTSET_DIMENSION_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/pointset/pointset_locator.h"

namespace Pastel
{

	//! Returns the dimension of a point-set.
	/*!
	Preconditions:
	Point has a default locator.
	*/
	template <typename PointSet>
	integer pointSetDimension(
		const PointSet& pointSet)
	{
		return pointSetLocator(pointSet).n();
	}

}

#endif
