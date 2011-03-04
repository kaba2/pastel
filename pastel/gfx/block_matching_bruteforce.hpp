#ifndef PASTEL_BLOCK_MATCHING_BRUTEFORCE_HPP
#define PASTEL_BLOCK_MATCHING_BRUTEFORCE_HPP

#include "pastel/gfx/block_matching_bruteforce.h"

#include "pastel/geometry/intersect_alignedbox_alignedbox.h"
#include "pastel/geometry/distance_point_point.h"

#include "pastel/sys/smallfixedset.h"
#include "pastel/sys/keyvalue.h"
#include "pastel/sys/countingiterator.h"
#include "pastel/sys/rectangleiterator.h"
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
		RectangleIterator<N> iter(blockExtent);
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
	Array<integer> matchBlockBrute(
		const Array<Real, N>& image,
		const Vector<integer, N>& blockExtent,
		const Vector<integer, N>& neighborhood,
		const Vector<integer, N>& step,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection)
	{
		ENSURE_OP(kNearest, >, 0);
		ENSURE_OP(maxDistance, >=, 0);
		ENSURE(allGreater(blockExtent, 0));
		ENSURE(allGreaterEqual(neighborhood, 0));
		ENSURE(allGreater(step, 0));

		// The nearest-neighbor blocks are identified with
		// the minimum point, and are further identified with
		// the linear index of that point in the 'image' array.
		// -1 encodes that a neighbor does not exists.

		const integer n = image.dimension();
		const integer m = product(blockExtent);

		const AlignedBox<integer, N> regionBox(
			Vector<integer, N>(ofDimension(n), 0),
			image.extent() - blockExtent + 1);

		const Vector<integer, N> steps =
			(regionBox.extent() + step - 1) / step;

		const integer blocks = product(steps);

		Array<integer> nearestSet(kNearest, image.size(), -1);
		if (kNearest == 0)
		{
			// Nothing else to do.
			return nearestSet;
		}

		std::vector<integer> indexSet;
		indexSet.reserve(blocks);
		{
			RectangleIterator<N> iter(steps);
			while(!iter.done())
			{
				indexSet.push_back(
					image.index(iter.position() * step));
				++iter;
			}
		}

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
		for (integer t = 0;t < blocks;++t)
		{
			const integer aIndex = 
				indexSet[t];

			const Vector<integer, N> aPosition =
				image.position(aIndex);
			
			// This is the distance to the current 
			// kth nearest neighbor candidate.
			Real kthDistance = maxDistance;

			aNearestSet.clear();

			neighborBox.min() = 
				aPosition - neighborhood;
			neighborBox.max() = 
				aPosition + neighborhood + 1;

			if (!intersect(regionBox, neighborBox, neighborBox))
			{
				continue;
			}

			RectangleIterator<N> bIter(
				neighborBox.min(), neighborBox.max());
			while(!bIter.done())
			{
				const Vector<integer, N>& bPosition =
					bIter.position();

				if (aPosition == bPosition)
				{
					// We want the block itself to be the
					// closest match, so we skip testing
					// it here, and automatically include it.
					++bIter;
					continue;
				}

				// Compute the distance between the
				// blocks A and B.
				const Real distance = blockDistance(
					image,
					aPosition,
					bPosition,
					blockExtent,
					kthDistance,
					normBijection);

				if (distance <= kthDistance)
				{
					// The distance computation was completed,
					// and therefore this block is a new candidate 
					// for the k-nearest neighbors.

					const integer bIndex =
						image.index(bPosition);
					aNearestSet.insert(
						keyValue(distance, bIndex));
					if (aNearestSet.full())
					{
						kthDistance = aNearestSet.back().key();
					}
				}

				++bIter;
			}

			// Copy the nearest neighbors to 'nearestSet'.

			// The nearest block is the block itself.
			nearestSet(0, aIndex) = aIndex;
			for (integer i = 0;i < aNearestSet.size();++i)
			{
				nearestSet(i + 1, aIndex) = aNearestSet[i].value();
			}
		}
		}

		return nearestSet;
	}

}

#endif
