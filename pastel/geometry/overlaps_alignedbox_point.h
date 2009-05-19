/*!
\file
\brief A function for testing overlap between an alignedBox and a point.
*/

#ifndef PASTEL_ALIGNEDBOXPOINT_H
#define PASTEL_ALIGNEDBOXPOINT_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	//! Tests if an alignedBox and a point overlap.

	/*!
	The alignedBox is considered to be closed and solid.
	*/

	template <int N, typename Real>
		bool overlaps(
			const AlignedBox<N, Real>& alignedBox,
			const Point<N, Real>& point);

}

#include "pastel/geometry/overlaps_alignedbox_point.hpp"

#endif
