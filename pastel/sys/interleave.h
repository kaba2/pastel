// Description: Interleaves subsequent sequences
// Documentation: stdext.txt

#ifndef PASTEL_INTERLEAVE_H
#define PASTEL_INTERLEAVE_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Interleaves subsequent sequences.
	/*!
	Preconditions:
	parts > 0
	range.size() % parts == 0
	
	Time complexity:
	O(n)
	Exactly n - 2 swaps are carried out.
	
	Space complexity:
	The space complexity of swapping two elements
	(usually O(1)).

	Example:
	The sequence 012345 with 3 parts 
	becomes 024135. With 2 parts it 
	becomes 031425.
	*/
	template <typename RandomAccessRange>
	void interleave(
		const RandomAccessRange& range,
		integer parts);

}

#include "pastel/sys/interleave.hpp"

#endif
