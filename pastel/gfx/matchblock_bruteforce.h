#ifndef PASTEL_MATCHBLOCK_BRUTEFORCE_H
#define PASTEL_MATCHBLOCK_BRUTEFORCE_H

#include "pastel/sys/array.h"
#include "pastel/sys/vector.h"

#include "pastel/math/normbijection_concept.h"

namespace Pastel
{

	//! Finds k-nearest neighbors for each block using brute force.
	/*!
	Preconditions:
	kNearest > 0
	maxDistance >= 0

	image:
	The image in which and whose blocks nearest neighbors
	are searched for.

	blockExtent:
	The extents of a rectangular block.

	kNearest:
	The number of nearest neighbors to find.

	maxDistance:
	The maximum distance in which to accept
	blocks as neighbors. Use 'infinity<Real>()'
	to remove this restriction.

	normBijection:
	The norm to use for distance measurement.
	See 'normbijection.txt'.

	A block is a rectangular subset of 'image'.
	*/

	template <typename Real, int N, typename NormBijection>
	Array<integer, 2> matchBlockBrute(
		const Array<Real, N>& image,
		const Vector<integer, N>& blockExtent,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection);

}

#include "pastel/gfx/matchblock_bruteforce.hpp"

#endif
