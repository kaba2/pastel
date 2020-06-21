// Description: Bounding aligned box of a point-set

#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_H
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/locator/locator_concept.h"

#include "pastel/geometry/shape/alignedbox.h"

namespace Pastel
{

	//! Bounding aligned box of a point set.
	template <PointSet_Concept_ PointSet>
	auto boundingAlignedBox(PointSet pointSet)
	-> AlignedBox<PointSet_Real<PointSet>, PointSet_Dimension<PointSet>::value>;

}

#include "pastel/geometry/bounding/bounding_alignedbox_pointset.hpp"

#endif
