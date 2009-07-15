// Description: Overlap tests between two capsules

#ifndef PASTEL_OVERLAPS_CAPSULE_CAPSULE_H
#define PASTEL_OVERLAPS_CAPSULE_CAPSULE_H

#include "pastel/geometry/capsule.h"

namespace Pastel
{

	//! Returns if two capsules overlap or not.
	/*!
	The capsules are considered closed and solid.
	*/

	template <int N, typename Real>
	bool overlaps(
		const Capsule<N, Real>& aCapsule,
		const Capsule<N, Real>& bCapsule);

}

#include "pastel/geometry/overlaps_capsule_capsule.hpp"

#endif
