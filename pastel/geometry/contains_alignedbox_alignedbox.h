// Description: Containment tests between aligned boxes

#ifndef PASTEL_CONTAINS_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTEL_CONTAINS_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Finds out if an aligned box is contained in another one.
	template <int N, typename Real>
	bool contains(
		const AlignedBox<N, Real>& outer,
		const AlignedBox<N, Real>& inner);

}

#include "pastel/geometry/contains_alignedbox_alignedbox.hpp"

#endif
