// Description: Bounding aligned box of a point-set

#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_H
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_H

#include "pastel/sys/input_concept.h"
#include "pastel/sys/locator_concept.h"

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Bounding aligned box of a point set.
	template <
		typename Input,
		typename Locator>
	auto boundingAlignedBox(
		Input pointSet,
		const Locator& locator)
	-> AlignedBox<typename Locator::Real, Locator::N>;

}

#include "pastel/geometry/bounding/bounding_alignedbox_pointset.hpp"

#endif
