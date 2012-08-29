// Description: Containment of an aligned box by another one

#ifndef PASTELGEOMETRY_CONTAINS_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTELGEOMETRY_CONTAINS_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Returns if an aligned box is contained in another one.
	/*!
	Note: an empty box is contained in every box (even in an empty box).
	*/
	template <typename Real, int N>
	bool contains(
		const AlignedBox<Real, N>& outer,
		const AlignedBox<Real, N>& inner);

}

#include "pastel/geometry/contains_alignedbox_alignedbox.hpp"

#endif
