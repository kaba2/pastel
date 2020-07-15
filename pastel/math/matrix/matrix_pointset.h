// Description: Matrix columns as a point-set
// Documentation: pointset.txt

#ifndef PASTELMATH_MATRIX_POINTSET_H
#define PASTELMATH_MATRIX_POINTSET_H

#include <pastel/sys/pointset/pointset_concept.h>
#include <pastel/sys/range/sparse_range.h>
#include <pastel/sys/range/interval_range.h>
#include <pastel/sys/locator/pointerrange_locator.h>

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Constructs a point-set from the columns of a matrix.
	/*
	Preconditions:
	The matrix is stored in column-major order.

	Optional arguments:

	dBegin, dEnd (0 <= integer <= set.rows()):
	The range of rows which use as the coordinates 
	of the points. 
	Default: 0, set.rows().
	*/
	template <
		typename Real,
		typename... ArgumentSet>
	decltype(auto) matrixPointSet(
		const MatrixView<Real>& set,
		ArgumentSet&&... argumentSet)
	{
		const integer dBegin = PASTEL_ARG_S(dBegin, 0);
		const integer dEnd = PASTEL_ARG_S(dEnd, set.rows());

		ENSURE_OP(dBegin, >=, 0);
		ENSURE_OP(dBegin, <, dEnd);
		ENSURE_OP(dEnd, <=, set.rows());

		return locationSet(
			sparseRange(
				intervalRange(set.data(), set.data() + set.size()), 
				set.rows()), 
			PointerRange_Locator<dreal>(dBegin, dEnd));
	}

}

#endif
