#ifndef PASTEL_ALIGNEDBOXPLANE_HPP
#define PASTEL_ALIGNEDBOXPLANE_HPP

#include "pastel/geometry/overlaps_alignedbox_plane.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/sys/point.h"
#include "pastel/geometry/plane.h"

namespace Pastel
{

	template <int N, typename Real>
		bool overlaps(
			const AlignedBox<N, Real>& alignedBox,
			const Plane<N, Real>& plane)
	{
		// For this test we first obtain the two corner points that
		// are maximally apart from each other w.r.t the plane
		// normal direction. Once we acquire these, the AlignedBox and
		// the Plane overlap iff these points are located on
		// different sides of the plane.

		// See "Geometric tools for computer graphics".

		PENSURE(alignedBox.dimension() == plane.dimension());

		const integer dimension = alignedBox.dimension();

		Point<N, Real> minimal(ofDimension(dimension));
		Point<N, Real> maximal(ofDimension(dimension));

		for (integer i = 0;i < dimension - 1;++i)
		{
			if (positive(plane.normal()[i]))
			{
				minimal[i] = alignedBox.min()[i];
				maximal[i] = alignedBox.max()[i];
			}
			else
			{
				minimal[i] = alignedBox.max()[i];
				maximal[i] = alignedBox.min()[i];
			}
		}

		const Real d(-dot(asVector(plane.position()),
			plane.normal()));

		if (positive(d))
		{
			minimal[dimension - 1] = alignedBox.min()[dimension - 1];
			maximal[dimension - 1] = alignedBox.max()[dimension - 1];
		}
		else
		{
			minimal[dimension - 1] = alignedBox.max()[dimension - 1];
			maximal[dimension - 1] = alignedBox.min()[dimension - 1];
		}

		// Are the extremal points on different sides
		// of the plane?

		if (positive(
			dot(minimal - plane.position(),
			plane.normal())) ^
			positive(
			dot(maximal - plane.position(),
			plane.normal())))
		{
			return false;
		}

		return true;
	}

}

#endif
