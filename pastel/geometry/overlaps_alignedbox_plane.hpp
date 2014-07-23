#ifndef PASTELGEOMETRY_ALIGNEDBOXPLANE_HPP
#define PASTELGEOMETRY_ALIGNEDBOXPLANE_HPP

#include "pastel/geometry/overlaps_alignedbox_plane.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/plane.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real, int N>
	bool overlaps(
		const AlignedBox<Real, N>& alignedBox,
		const Plane<Real, N>& plane)
	{
		PENSURE_OP(alignedBox.n(), ==, plane.n());

		integer n = alignedBox.n();

		// Find maximal and minimal signed distances
		// of the vertices in V along the normal vector
		// of the plane.

		// FIX: Box topologies are not handled correctly.
		// The box is now treated as closed. I do
		// not see how this could be handled.

		Real minDistance = 
			dot(alignedBox.min() - plane.position(), plane.normal());
		Real maxDistance = 
			minDistance;
		for (integer i = 0;i < n;++i)
		{
			Real deltaAxisDistance = 

				alignedBox.extent(i) * plane.normal()[i];
			if (deltaAxisDistance < 0)
			{
				minDistance += deltaAxisDistance;
			}
			else
			{
				maxDistance += deltaAxisDistance;
			}
		}

		return (minDistance <= 0 && maxDistance >= 0);
	}

}

#endif
