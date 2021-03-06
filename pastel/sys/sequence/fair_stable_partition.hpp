#ifndef PASTELSYS_FAIR_STABLE_PARTITION_HPP
#define PASTELSYS_FAIR_STABLE_PARTITION_HPP

#include "pastel/sys/sequence/fair_stable_partition.h"

#include "pastel/sys/trindicator/trindicator_concept.h"
#include "pastel/sys/range.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	//! Partitions a sequence fairly and stably.
	/*!
	Time complexity:
	O(n)
	where
	n = boost::size(elementSet).

	Space complexity: O(n)

	Stable means that the ordering of elements is preserved.
	Fair means that half of the elements with a zero indicator 
	are interpreted as negative, and the others as positive.
	When n >= 2, and at least one point has zero indicator,
	both sets will be non-empty.
	*/
	template <
		ranges::forward_range Range,
		typename Trindicator>
	ranges::iterator_t<Range> 
		fairStablePartition(
			Range&& elementSet,
			const Trindicator& trindicator)
		{
			using Iterator = ranges::iterator_t<Range>;
			using Type = ranges::range_value_t<Range>;
		
			integer zeros = 0;
			integer negatives = 0;
			integer positives = 0;

			for (auto&& element : elementSet)
			{
				const integer i = trindicator(element);

				if (i > 0)
				{
					++positives;
				}
				else if (i < 0)
				{
					++negatives;
				}
				else
				{
					++zeros;
				}
			}

			integer negativeZeros = (zeros + 1) / 2;
			if (negatives > positives && (zeros & 1) == 1)
			{
				--negativeZeros;
			}

			const integer positiveZeros = zeros - negativeZeros;

			// We will first move the elements with a
			// positive indicator into this temporary set.
			// At the end of the algorithm we will move 
			// these elements to the end of the original 
			// sequence.
			std::vector<Type> positiveSet;
			positiveSet.reserve(positives + positiveZeros);

			// The end of the negative set. This will
			// be tracked throughout the algorithm.
			Iterator negativeEnd = std::begin(elementSet);

			// The number of zero-indicators seen
			// thus far.
			integer zerosThusfar = 0;
			for (auto&& element : elementSet)
			{
				// Find out the indicator of the
				// current element.
				integer i = trindicator(element);

				if (i == 0)
				{
					// Partition the zero-indicators fairly.
					i = (zerosThusfar < negativeZeros) ? -1 : 1;

					// The element has zero indicator.
					++zerosThusfar;
				}

				if (i < 0)
				{
					// This element is negative. Move it to
					// the end of the negative set.
					*negativeEnd = std::move(element);

					// Update the end of the negative set.
					++negativeEnd;
				}
				else
				{
					// This element is positive. Move it to 
					// the temporary positive set.
					positiveSet.emplace_back(std::move(element));
				}
			}

			// Move the temporary positive set after the
			// negative set.
			std::move(positiveSet.begin(), positiveSet.end(),
				negativeEnd);

			// Return the end of the negative set.
			return negativeEnd;
		}

}

#endif
