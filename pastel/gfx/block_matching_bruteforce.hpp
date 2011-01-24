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
	Array<integer, 2> matchBlockBrute2(
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

		std::vector<Vector<integer, N> > deltaSet;
		deltaSet.push_back(Vector<integer, N>(-1, -1));
		deltaSet.push_back(Vector<integer, N>(-1, 0));
		deltaSet.push_back(Vector<integer, N>(0, -1));

		const integer deltas = deltaSet.size();

#pragma omp parallel
		{
		// We seek for k-nearest neighbors for
		// block A. The 'aNearestSet' keeps track
		// of the k best candidates.
		// We only seek for k - 1 neighbors, since
		// we will automatically include the block itself.
		
		UnorderedSet<integer> reservedSet;
		std::vector<integer> aNearestSet;

		AlignedBox<integer, N> neighborBox(
			ofDimension(n));

		// For each block A in the region...
#pragma omp for
		for (integer y = regionBox.min().y();y < regionBox.max().y();++y)
		{
			for (integer x = regionBox.min().x();x < regionBox.max().x();++x)
			{
				aNearestSet.clear();
				reservedSet.clear();

				const Vector<integer, N> aPosition(x, y);

				neighborBox.min() = 
					aPosition - neighborhood;
				neighborBox.max() = 
					aPosition + neighborhood;

				if (!intersect(regionBox, neighborBox, neighborBox))
				{
					continue;
				}

				const integer aIndex = 
					image.index(aPosition);

				integer predicted = 0;
				for (integer j = 0;j < nearestSet.width();++j)
				{
					for (integer i = 0;i < deltas;++i)
					{
						const Vector<integer, N>& delta = deltaSet[i];
						const Vector<integer, N> position =
							aPosition + delta;
						if (anyLess(position, regionBox.min()) ||
							anyGreaterEqual(position, regionBox.max()))
						{
							continue;							
						}

						const integer deltaIndex =
							image.index(position);
						const integer deltaNeighborIndex =
							nearestSet(j, deltaIndex);

						if (deltaNeighborIndex < 0)
						{
							continue;
						}
						
						const Vector<integer, N> predictedPosition =
							image.position(deltaNeighborIndex) - delta;
						if (anyLess(predictedPosition, regionBox.min()) ||
							anyGreaterEqual(predictedPosition, regionBox.max()))
						{
							continue;
						}

						const Real distance = blockDistance(
							image, 
							aPosition, predictedPosition,
							blockExtent, maxDistance,
							normBijection);

						if (distance <= maxDistance)
						{
							const integer predictedIndex =
								image.index(predictedPosition);
							nearestSet(j, aIndex) = predictedIndex;
							ASSERT(reservedSet.count(predictedIndex) == 0);
							reservedSet.insert(predictedIndex);
							++predicted;
							break;
						}
					}
				}

				if (predicted < kNearest - 1)
				{
					GridIterator<N> bIter(
						neighborBox.max() - neighborBox.min());
					while(!bIter.done())
					{
						const Vector<integer, N> bPosition =
							neighborBox.min() + bIter.position();
						const integer bIndex =
							image.index(bPosition);

						if (bPosition == aPosition ||
							reservedSet.count(bIndex))
						{
							// We want the block itself to be the
							// closest match, so we skip testing
							// it here, and automatically include it.
							++bIter;
							continue;
						}

						const Real distance = blockDistance(
							image, 
							aPosition, bPosition,
							blockExtent, maxDistance,
							normBijection);

						if (distance <= maxDistance)
						{
							// The distance computation was completed,
							// and therefore this block is a new candidate 
							// for the k-nearest neighbors.

							aNearestSet.push_back(bIndex);
						}

						++bIter;
					}
				}

				// Copy the nearest neighbors to 'nearestSet'.
				const integer toCopy = 
					std::min((integer)aNearestSet.size(), kNearest - 1);

				// The nearest block is the block itself.
				integer picked = 0;
				nearestSet(0, aIndex) = aIndex;
				for (integer i = 0;i < toCopy;++i)
				{
					integer& neighbor = nearestSet(i + 1, aIndex);

					if (neighbor < 0)
					{
						using std::swap;
						const integer randomIndex = 
							randomInteger(picked, aNearestSet.size() - 1);
						ASSERT(randomIndex >= 0 && randomIndex < aNearestSet.size());
						swap(aNearestSet[randomIndex],
							aNearestSet.front());
						neighbor = aNearestSet.front();
						++picked;
					}
				}
			}
		}
		}

		return nearestSet;
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
						for (integer j = 0;j < blockExtent.x();++j)
						{
							for (integer i = 0;i < blockExtent.y();++i)
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
