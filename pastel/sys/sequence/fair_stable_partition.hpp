#ifndef PASTELSYS_FAIR_STABLE_PARTITION_HPP
#define PASTELSYS_FAIR_STABLE_PARTITION_HPP

#include "pastel/sys/sequence/fair_stable_partition.h"
#include "pastel/sys/trindicator/trindicator_concept.h"
#include "pastel/sys/ensure.h"

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

			integer zeros = 0;

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
						++zeros;
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

			if (n >= 2 && zeros > 0)
			{
				// There is the possibility that all points
				// are assigned to the same set, although
				// there is at least one boundary point.
				//
				// In this case we case we want the set B 
				// (positive/negative) to get the boundary point. 
				// For example, with kd-trees it is important 
				// that splitting a node always decreases the 
				// number of points in a node.

				if (negativeEnd == end)
				{
					// All points are in negative set.
					
					// Since we have at least one
					// boundary point, there is exactly
					// one boundary point, since we
					// alternate the assignment in the
					// loop above.
					ASSERT_OP(zeros, ==, 1);

					// Move that boundary point into
					// the positive set.
					--negativeEnd;
				}
				else if (negativeEnd == begin)
				{
					// All points are in positive set.
					ASSERT_OP(zeros, ==, 1);
					++negativeEnd;
				}
			}

			// Return the end of the negative set.
			return negativeEnd;
		}

}

#endif
