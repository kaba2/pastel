#ifndef PASTEL_BLOCK_MATCHING_BRUTEFORCE_HPP
#define PASTEL_BLOCK_MATCHING_BRUTEFORCE_HPP

#include "pastel/gfx/block_matching_bruteforce.h"

#include "pastel/geometry/intersect_alignedbox_alignedbox.h"
#include "pastel/geometry/distance_point_point.h"

#include "pastel/sys/smallfixedset.h"
#include "pastel/sys/keyvalue.h"
#include "pastel/sys/countingiterator.h"
#include "pastel/sys/griditerator.h"
#include "pastel/sys/stdext_subset.h"
#include "pastel/sys/unorderedset.h"

#include <boost/range.hpp>

#include <algorithm>

namespace Pastel
{

	template <int N, typename Real, typename NormBijection>
	Real blockDistance(
		const Array<Real, N>& image,
		const Vector<integer, N>& aPosition,
		const Vector<integer, N>& bPosition,
		const Vector<integer, N>& blockExtent,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection)
	{
		// Compute the distance between the
		// blocks A and B.

		Real distance = 0;
		GridIterator<N> iter(blockExtent);
		while(!iter.done())
		{
			distance = normBijection.addAxis(
				distance, 
				normBijection.signedAxis(
				image(aPosition + iter.position()) - 
				image(bPosition + iter.position())));
			if (distance > maxDistance)
			{
				break;
			}

			++iter;
		}

		return distance;
	}

	template <typename Real, int N, typename NormBijection>
	Array<integer, 2> matchBlockBrute(
		const Array<Real, N>& image,
		const Vector<integer, N>& blockExtent,
		const Vector<integer, N>& neighborhood,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection)
	{
		PASTEL_STATIC_ASSERT(N == 2);

		ENSURE_OP(kNearest, >, 0);
		ENSURE(maxDistance >= 0);

		// The nearest-neighbor blocks are identified with
		// the minimum point, and are further identified with
		// the linear index of that point in the 'image' array.
		// -1 encodes that a neighbor does not exists.

		Array<integer> nearestSet(kNearest, image.size(), -1);
		if (kNearest == 0)
		{
			// Nothing else to do.
			return nearestSet;
		}

		const integer n = image.dimension();
		const integer m = product(blockExtent);

		const AlignedBox<integer, N> regionBox(
			Vector<integer, N>(ofDimension(n), 0),
			image.extent() - blockExtent + 1);
		
#pragma omp parallel
		{
		// We seek for k-nearest neighbors for
		// block A. The 'aNearestSet' keeps track
		// of the k best candidates.
		// We only seek for k - 1 neighbors, since
		// we will automatically include the block itself.
		SmallFixedSet<KeyValue<Real, integer> > aNearestSet(kNearest - 1);

		AlignedBox<integer, N> neighborBox(
			ofDimension(n));

		// For each block A in the region...
#pragma omp for
		for (integer y = regionBox.min().y();y < regionBox.max().y();++y)
		{
			for (integer x = regionBox.min().x();x < regionBox.max().x();++x)
			{
				// This is the distance to the current 
				// kth nearest neighbor candidate.
				Real kthDistance = maxDistance;

				aNearestSet.clear();

				const Vector<integer, N> aPosition(x, y);

				neighborBox.min() = 
					aPosition - neighborhood;
				neighborBox.max() = 
					aPosition + neighborhood;

				if (!intersect(regionBox, neighborBox, neighborBox))
				{
					continue;
				}

				for (integer v = neighborBox.min().y();v < neighborBox.max().y();++v)
				{
					for (integer u = neighborBox.min().x();u < neighborBox.max().x();++u)
					{
						if (x == u && y == v)
						{
							// We want the block itself to be the
							// closest match, so we skip testing
							// it here, and automatically include it.
							continue;
						}

						// Compute the distance between the
						// blocks A and B.

						Real distance = 0;
						for (integer i = 0;i < blockExtent.x();++i)
						{
							for (integer j = 0;j < blockExtent.y();++j)
							{
								distance = normBijection.addAxis(
									distance, 
									normBijection.signedAxis(
									image(x + i, y + j) - image(u + i, v + j)));
								if (distance > kthDistance)
								{
									break;
								}
							}
							if (distance > kthDistance)
							{
								break;
							}
						}

						if (distance <= kthDistance)
						{
							// The distance computation was completed,
							// and therefore this block is a new candidate 
							// for the k-nearest neighbors.

							const integer bIndex =
								image.index(Vector<integer, N>(u, v));
							aNearestSet.insert(
								keyValue(distance, bIndex));
							if (aNearestSet.full())
							{
								kthDistance = aNearestSet.back().key();
							}
						}
					}
				}

				// Copy the nearest neighbors to 'nearestSet'.
				const integer aIndex = 
					image.index(Vector<integer, N>(x, y));

				// The nearest block is the block itself.
				nearestSet(0, aIndex) = aIndex;
				for (integer i = 0;i < aNearestSet.size();++i)
				{
					nearestSet(i + 1, aIndex) = aNearestSet[i].value();
				}
			}
		}
		}

		return nearestSet;
	}

}

#endif
