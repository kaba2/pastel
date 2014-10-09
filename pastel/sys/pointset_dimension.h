// Description: Dimension of a point-set

#ifndef PASTELSYS_POINTSET_DIMENSION_H
#define PASTELSYS_POINTSET_DIMENSION_H

#include "pastel/sys/pointset_concept.h"
#include "pastel/sys/pointset_locator.h"

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
