// Description: Low discrepancy sequences
// Detail: Allows to produce van Der Corput, Halton, Hammersley etc sequences.

#ifndef PASTEL_LOWDISCREPANCY_H
#define PASTEL_LOWDISCREPANCY_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Reverses the order of bits.
	uint32 reverseBits(uint32 input);

	//! Returns the n:th element of the base-2 Van Der Corput sequence.
	/*!
	Preconditions:
	n >= 0
	*/

	template <typename Real>
	Real vanDerCorputSequence(integer n);

	//! Returns the n:th element of the base-'base' Van Der Corput sequence.
	/*!
	Preconditions:
	n >= 0
	base >= 2
	*/

	template <typename Real>
	Real vanDerCorputSequence(integer n, integer base);

	//! Returns the n:th element of the Halton sequence.
	/*!
	Preconditions:
	n >= 0
	for all i e [0, N[: bases[i] >= 2
	The elements of 'bases' should be coprime to each other (not enforced!).
	*/

	template <typename Real, int N>
	Vector<Real, N> haltonSequence(integer n, const Vector<integer, N>& bases);

	//! Returns the n:th element of the Hammersley sequence.
	/*!
	Preconditions:
	size >= 1
	0 <= n <= size
	for all i e [0, N[: bases[i] >= 2
	The elements of 'bases' should be coprime to each other (not enforced!).

	The Hammersley sequence improves upon the discrepancy of the
	Hammersley sequence in the case you know in advance the number of
	samples you wish to draw.
	*/

	template <typename Real, int N>
	Vector<Real, ModifyN<N, N + 1>::Result> hammersleySequence(
		integer n, const Vector<integer, N>& bases, integer size);

}

#include "pastel/math/lowdiscrepancy.hpp"

#endif
