#ifndef PASTEL_MATCHBLOCK_BRUTEFORCE_HPP
#define PASTEL_MATCHBLOCK_BRUTEFORCE_HPP

#include "pastel/gfx/matchblock_bruteforce.h"

#include "pastel/sys/smallfixedset.h"
#include "pastel/sys/keyvalue.h"

namespace Pastel
{

	template <typename Real, int N, typename NormBijection>
	Array<integer, 2> matchBlockBrute(
		const Array<Real, N>& image,
		const Vector<integer, N>& blockExtent,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection)
	{
		ENSURE_OP(kNearest, >, 0);
		ENSURE(maxDistance >= 0);

		// The nearest-neighbor blocks are identified with
		// the minimum point, and are further identified with
		// the linear index of that point in the 'image' array.
		// -1 encodes that a neighbor does not exists.

		Array<integer> nearestSet(kNearest, image.size(), -1);

		typedef ConstSubArray<Real, N> Region;
		typedef typename Region::ConstIterator ConstIterator;

		const Region region =
			image(Vector<integer, N>(ofDimension(image.dimension()), 0), 
			image.extent() - blockExtent);

#pragma omp parallel
		{
		// We seek for k-nearest neighbors for
		// block A. The 'aNearestSet' keeps track
		// of the k best candidates.
		SmallFixedSet<KeyValue<Real, integer> > aNearestSet(kNearest); 

		// For each block A in the region...
		const ConstIterator regionEnd = region.end();
#pragma omp for
		for (integer a = 0;a < region.size();++a)
		{
			// This is the distance to the current 
			// kth nearest neighbor candidate.
			Real kthDistance = maxDistance;

			aNearestSet.clear();

			const ConstIterator aIter = region.begin(a);

			// This is the sub-array for the block A.
			const Region aBlock = 
				image(aIter.position(),
				aIter.position() + blockExtent);

			ConstIterator bIter = region.begin();
			while (bIter != regionEnd)
			{
				// This is the sub-array for the block B.
				const Region bBlock =
					image(bIter.position(),
					bIter.position() + blockExtent);

				// Compute the distance between the
				// blocks A and B.

				ConstIterator iIter = aBlock.begin();
				const ConstIterator iEnd = aBlock.end();
				ConstIterator jIter = bBlock.begin();
				Real distance = 0;
				while (iIter != iEnd)
				{
					distance = normBijection.addAxis(
						distance, 
						normBijection.signedAxis(*iIter - *jIter));
					if (distance > kthDistance)
					{
						// The block is farther than the currently
						// kth nearest neighbor. We need
						// not complete the computation.
						break;
					}

					++iIter;
					++jIter;
				}
				if (iIter == iEnd)
				{
					// The distance computation was completed,
					// and therefore this block is a new candidate 
					// for the k-nearest neighbors. 

					const integer bIndex = 
						&*bIter - &*region.begin();
					aNearestSet.insert(
						keyValue(distance, bIndex));
					if (aNearestSet.full())
					{
						kthDistance = aNearestSet.back().key();
					}
				}

				// Next block B.
				++bIter;
			}

			// Copy the nearest neighbors to 'nearestSet'.
			const integer aIndex = 
					&*aIter - &*region.begin();
			for (integer i = 0;i < aNearestSet.size();++i)
			{
				nearestSet(i, aIndex) = aNearestSet[i].value();
			}
		}
		}

		return nearestSet;
	}

}

#endif
