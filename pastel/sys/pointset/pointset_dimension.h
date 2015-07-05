// Description: Dimension of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_DIMENSION_H
#define PASTELSYS_POINTSET_DIMENSION_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/pointset/pointset_locator.h"

namespace Pastel
{

	//! Returns the dimension of a point-set.
	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	integer pointSetDimension(
		const PointSet& pointSet)
	{
		return pointSetLocator(pointSet).n();
	}

	//! Returns the compile-time dimension of a point-set.
	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	using PointSet_Dimension =
		Locator_N<PointSet_Locator<PointSet>>;

}

#endif
