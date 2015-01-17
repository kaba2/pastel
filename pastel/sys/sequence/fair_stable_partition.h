// Description: Fair stable partitioning
// Documentation: sequence_algorithms.txt

#ifndef PASTELSYS_FAIR_STABLE_PARTITION_H
#define PASTELSYS_FAIR_STABLE_PARTITION_H

#include "pastel/sys/trindicator/trindicator_concept.h"
#include "pastel/sys/range.h"

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
	
	Specifically, an element is interpreted as negative or 
	positive by that
	
	1) if the positive set is currently empty, 
	then the element is positive,
	
	2) if the negative set is currently empty, 
	then the element is negative, and
	
	3) otherwise the element is alternately 
	positive or negative.
	*/
	template <
		typename Range,
		typename Trindicator>
	typename boost::range_iterator<Range>::type 
		fairStablePartition(
			const Range& elementSet,
			const Trindicator& trindicator);

}

#include "pastel/sys/sequence/fair_stable_partition.hpp"

#endif
