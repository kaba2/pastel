/*!
\file
\brief A function for computing if one aligned box is contained in another.
*/

#ifndef PASTELGEOMETRY_CONTAINS_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTELGEOMETRY_CONTAINS_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Returns true if the 'inner' is contained in the 'outer'.

	template <int N, typename Real>
	bool contains(
		const AlignedBox<N, Real>& outer,
		const AlignedBox<N, Real>& inner);

}

#include "pastel/geometry/contains_alignedbox_alignedbox.hpp"

#endif
