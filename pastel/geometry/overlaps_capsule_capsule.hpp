#ifndef PASTEL_OVERLAPS_CAPSULE_CAPSULE_HPP
#define PASTEL_OVERLAPS_CAPSULE_CAPSULE_HPP

#include "pastel/geometry/overlaps_capsule_capsule.h"
#include "pastel/geometry/distance_segment_segment.h"

namespace Pastel
{

	template <int N, typename Real>
	bool overlaps(
		const Capsule<N, Real>& aCapsule,
		const Capsule<N, Real>& bCapsule)
	{
		const Real dist2 = distance2(
			aCapsule.segment(),
			bCapsule.segment());

		const Real sumRadius = aCapsule.radius() + bCapsule.radius();

		return dist2 < sumRadius * sumRadius;
	}

}

#endif
