#ifndef PASTEL_OVERLAPS_ALIGNEDBOX_POINT_HPP
#define PASTEL_OVERLAPS_ALIGNEDBOX_POINT_HPP

#include "pastel/geometry/overlaps_alignedbox_point.h"
#include "pastel/geometry/overlaps_alignedbox_sphere.h"
#include "pastel/geometry/alignedbox.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	template <int N, typename Real>
	bool overlaps(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point)
	{
		// This overlap test is broken into N 1-dimensional tests.
		// Each test is just a simple number-in-a-range test
		// using coordinates on some axis.
		// If even one of these tests fail, the objects are
		// guaranteed not to overlap, and we can exit early.
		// If all the tests pass, the objects overlap.

		PENSURE_OP(alignedBox.dimension(), ==, point.dimension());

		const integer dimension = alignedBox.dimension();

		for (integer i = 0;i < dimension;++i)
		{
			// Test if the i:th coordinate of the point
			// lies beyond the closed i:th coordinate range
			// of the aligned box.

			if (point[i] < alignedBox.min()[i] ||
				point[i] > alignedBox.max()[i])
			{
				return false;
			}
		}

		return true;
	}

}

#endif
