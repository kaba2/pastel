// Description: Containment of an aligned box by another one

#ifndef PASTELGEOMETRY_CONTAINS_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTELGEOMETRY_CONTAINS_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/sys/vector/vector.h"

namespace Pastel
{

	//! Returns if an aligned box is contained in another one.
	/*!
	Note: an empty box is contained in every box (even in an empty box).
	*/
	template <typename Real, int N>
	bool contains(
		const AlignedBox<Real, N>& outer,
		const AlignedBox<Real, N>& inner)
	{
		PENSURE_OP(outer.n(), ==, inner.n());

		integer n = outer.n();
		for (integer i = 0;i < n;++i)
		{
			// forall i in [0, i[:

			// !inner.empty(i) && !outer.empty(i)

			if (inner.empty(i))
			{
				// The empty box is contained in every box.
				return true;
			}

			if (outer.empty(i))
			{
				// The outer box is empty.

				for (integer j = i + 1;j < n;++j)
				{
					if (inner.empty(j))
					{
						// The empty box is contained in
						// an empty box.
						return true;
					}
				}

				// Since the inner box is not empty
				// but the outer box is, there can't 
				// be containment.
				return false;
			}

			if (!containsPositiveHalfspace(
				outer.min()[i], outer.minTopology()[i],
				inner.min()[i], inner.minTopology()[i]))
			{
				// The inner box extends outside the 
				// minimum of the outer box.
				return false;
			}

			if (!containsNegativeHalfspace(
				outer.max()[i], outer.maxTopology()[i],
				inner.max()[i], inner.maxTopology()[i]))
			{
				// The inner box extends outside the 
				// maximum of the outer box.
				return false;
			}
		}

		// The inner box is contained in the outer box.
		return true;
	}

}

#endif
