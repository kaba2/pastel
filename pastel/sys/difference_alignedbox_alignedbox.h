// Description: Set-difference between aligned boxes
// Documentation: alignedbox.txt

#ifndef PASTEL_DIFFERENCE_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTEL_DIFFERENCE_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/sys/alignedbox.h"
#include "pastel/sys/reporter_concept.h"

namespace Pastel
{

	//! Computes the set-difference between two aligned boxes.
	/*!
	Preconditions:
	aBox.dimension() == bBox.dimension()

	Returns:
	The number of reported boxes.

	The set-difference A\B between two aligned boxes A and B
	is reported by at most 2n aligned boxes whose union it is.
	The interiors of these boxes do not intersect.
	*/
	template <
		typename Real, int N_A, int N_B,
		typename AlignedBox_Reporter>
	integer difference(
		const AlignedBox<Real, N_A>& aBox,
		const AlignedBox<Real, N_B>& bBox,
		AlignedBox_Reporter report);

	//! Computes the symmetric set-difference between two aligned boxes.
	/*!
	Preconditions:
	aBox.dimension() == bBox.dimension()

	The symmetric set-difference A(+)B = A\B union B\A between two 
	aligned boxes A and B is reported by at most 4n aligned boxes 
	whose union it is. The interiors of these boxes do not intersect.
	*/
	template <
		typename Real, int N_A, int N_B,
		typename AlignedBox_Reporter>
	integer symmetricDifference(
		const AlignedBox<Real, N_A>& aBox,
		const AlignedBox<Real, N_B>& bBox,
		AlignedBox_Reporter report);

}

#include "pastel/sys/difference_alignedbox_alignedbox.hpp"

#endif
