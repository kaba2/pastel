// Description: Traverses points in an integral aligned box.
// Documentation: grid_traversal.txt

#ifndef PASTELSYS_FOR_EACH_POINT_H
#define PASTELSYS_FOR_EACH_POINT_H

#include "pastel/sys/alignedbox.h"
#include "pastel/sys/output/output_concept.h"

namespace Pastel
{

	//! Traverses points in an integral aligned box.
	/*!
	RowMajor:
	Whether to traverse the points in row-major order.
	The alternative is column-major order.

	box:
	The traversed points are given by the half-open box
	[box.min(), box.max()). If this set is empty, then 
	nothing is done.

	report:
	An output called with a point of the type
	Vector<integer, N>.
	*/
	template <
		bool RowMajor = true,
		int N,
		typename Vector_Output>
	void forEachPoint(
		AlignedBox<integer, N> box,
		Vector_Output report);

}

#include "pastel/sys/for_each_point.hpp"

#endif
