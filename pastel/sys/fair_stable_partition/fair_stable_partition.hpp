#ifndef PASTELSYS_FAIR_STABLE_PARTITION_HPP
#define PASTELSYS_FAIR_STABLE_PARTITION_HPP

#include "pastel/sys/fair_stable_partition.h"
#include "pastel/sys/trindicator_concept.h"

namespace Pastel
{

	template <
		typename Range,
		typename Trindicator>
	typename boost::range_iterator<Range>::type 
		fairStablePartition(
			const Range& elementSet,
			const Trindicator& trindicator)
		{
			using Iterator = typename boost::range_iterator<Range>::type;
			using Type = typename boost::range_value<Range>::type;

			// Compute the number of elements.
			integer n = boost::size(elementSet);
			
			// We will first move the elements with a
			// positive indicator into this temporary set.
			// At the end of the algorithm we will move 
			// these elements to the end of the original 
			// sequence.
			std::vector<Type> positiveSet;
			positiveSet.reserve(n);

			// To make the partition 'fair', we will interpret
			// every second element with a zero indicator
			// as a negative element, and the rest as 
			// positive elements. 
			bool zeroIsPositive = true;

			Iterator begin = std::begin(elementSet);
			Iterator end = std::end(elementSet);

			// The end of the negative set. This will
			// be tracked throughout the algorithm.
			Iterator negativeEnd = begin;
			
			Iterator current = begin;
			while (current != end)
			{
				// Find out the indicator of the
				// current element.
				integer i = trindicator(*current);

				if (i == 0)
				{
					// The element has a zero indicator.
					// See if this element is to be interpreted
					// as a negative or a positive element.

					if (negativeEnd == begin)
					{
						// The negative set is empty.
						// Treat as negative.
						i = -1;

						// The next one will be positive.
						zeroIsPositive = true;
					}
					else if (positiveSet.empty())
					{
						// The positive set is empty.
						// Treat as positive.
						i = 1;

						// The next one will be negative.
						zeroIsPositive = false;
					}
					else
					{
						// Neither set is empty, so 
						// alternate between the sets.
						i = zeroIsPositive ? 1 : -1;
						zeroIsPositive = !zeroIsPositive;
					}
				}

				if (i < 0)
				{
					// This element is negative. Move it to
					// the end of the negative set.
					*negativeEnd = std::move(*current);

					// Update the end of the negative set.
					++negativeEnd;
				}
				else
				{
					// This element is positive. Move it to 
					// the temporary positive set.
					positiveSet.emplace_back(std::move(*current));
				}

				// Proceed to the next element.
				++current;
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
