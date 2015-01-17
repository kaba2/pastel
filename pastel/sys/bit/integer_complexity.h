// Description: Complexity of an integer
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_INTEGER_COMPLEXITY_H
#define PASTELSYS_INTEGER_COMPLEXITY_H

#include "pastel/sys/integer/finite_integer_concept.h"

namespace Pastel
{

	//! Returns whether 'left' should be added before 'right' in skip-fast trie.
	/*!
	Time complexity: O(1)
	Exception safety: strong
	*/
	template <typename Finite_Integer>
	bool lessComplexInteger(
		const Finite_Integer& left,
		const Finite_Integer& right);

	//! Returns the complexity of an integer.
	/*!
	Time complexity: O(log(bits(that)))
	Exception safety: strong

	The complexity of an integer is the number of 01 and 10
	pairs in the bit-expansion of the integer.
	*/
	template <typename Finite_Integer>
	integer integerComplexity(const Finite_Integer& that);

}

#include "pastel/sys/bit/integer_complexity.hpp"

#endif
