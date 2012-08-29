// Description: Generation of a random subset
// Documentation: sequence_algorithms.txt

#ifndef PASTELSYS_RANDOM_SUBSET_H
#define PASTELSYS_RANDOM_SUBSET_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Generates a random subset
	/*!
	Preconditions:
	k >= 0
	k <= std::distance(begin, end)

	Postconditions:
	[begin, begin + k[ contains a random k-subset.
	*/

	template <typename RandomAccess_Iterator>
	void randomSubset(
		RandomAccess_Iterator begin,
		RandomAccess_Iterator end,
		integer k);

}

#include "pastel/sys/random_subset.hpp"

#endif
