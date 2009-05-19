/*!
\file
\brief A function for testing overlap between an alignedBox and a plane.
*/

#ifndef PASTEL_OVERLAPS_ALIGNEDBOX_PLANE_H
#define PASTEL_OVERLAPS_ALIGNEDBOX_PLANE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/plane.h"

namespace Pastel
{

	//! Tests if an alignedBox and a plane overlap.

	/*!
	The alignedBox is considered to be closed and solid.
	*/

	template <int N, typename Real>
		bool overlaps(
			const AlignedBox<N, Real>& alignedBox,
			const Plane<N, Real>& plane);

}

#include "pastel/geometry/overlaps_alignedbox_plane.hpp"

#endif
