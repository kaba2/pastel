// Description: Bounding aligned box of a point-set

#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_H
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/locator/locator_concept.h"

#include "pastel/geometry/shape/alignedbox.h"

namespace Pastel
{

	//! Bounding aligned box of a point set.
	template <
		typename PointSet,
		Requires<Models<PointSet, PointSet_Concept>> = 0
	>
	auto boundingAlignedBox(PointSet pointSet)
	-> AlignedBox<PointSet_Real<PointSet>, PointSet_N<PointSet>::value>;

}

#include "pastel/geometry/bounding/bounding_alignedbox_pointset.hpp"

#endif
