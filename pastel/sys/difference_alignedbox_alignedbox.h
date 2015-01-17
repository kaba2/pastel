// Description: Set-difference between aligned boxes
// Documentation: alignedbox.txt

#ifndef PASTELSYS_DIFFERENCE_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTELSYS_DIFFERENCE_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/sys/alignedbox.h"
#include "pastel/sys/output/output_concept.h"

namespace Pastel
{

	//! Computes the set-difference between two aligned boxes.
	/*!
	Preconditions:
	aBox.n() == bBox.n()

	Returns:
	The number of reported boxes.

	The set-difference A\B between two aligned boxes A and B
	is reported by at most 2n aligned boxes whose union it is.
	The interiors of these boxes do not intersect.
	*/
	template <
		typename Real, int N_A, int N_B,
		typename AlignedBox_Output>
	integer difference(
		const AlignedBox<Real, N_A>& aBox,
		const AlignedBox<Real, N_B>& bBox,
		AlignedBox_Output report);

	//! Computes the symmetric set-difference between two aligned boxes.
	/*!
	Preconditions:
	aBox.n() == bBox.n()

	The symmetric set-difference A(+)B = A\B union B\A between two 
	aligned boxes A and B is reported by at most 4n aligned boxes 
	whose union it is. The interiors of these boxes do not intersect.
	*/
	template <
		typename Real, int N_A, int N_B,
		typename AlignedBox_Output>
	integer symmetricDifference(
		const AlignedBox<Real, N_A>& aBox,
		const AlignedBox<Real, N_B>& bBox,
		AlignedBox_Output report);

}

#include "pastel/sys/difference_alignedbox_alignedbox.hpp"

#endif
