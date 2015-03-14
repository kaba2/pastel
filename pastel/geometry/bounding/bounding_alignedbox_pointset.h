// Description: Bounding aligned box of a point-set

#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_H
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_H

#include "pastel/sys/input/input_concept.h"
#include "pastel/sys/locator/locator_concept.h"

#include "pastel/geometry/shape/alignedbox.h"

namespace Pastel
{

	//! Bounding aligned box of a point set.
	template <
		typename Input,
		typename Locator,
		typename = 
			Requires<
				Models<Input, Input_Concept>, 
				Models<Locator, Locator_Concept>
			>
	>
	auto boundingAlignedBox(
		Input pointSet,
		const Locator& locator)
	-> AlignedBox<Locator_Real<Locator>, Locator::N>;

}

#include "pastel/geometry/bounding/bounding_alignedbox_pointset.hpp"

#endif
