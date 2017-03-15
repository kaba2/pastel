// Documentation: Matrix columns as a point-set

#ifndef PASTELMATH_MATRIX_POINTSET_H
#define PASTELMATH_MATRIX_POINTSET_H

#include <pastel/sys/pointset/pointset_concept.h>
#include <pastel/sys/set/sparse_set.h>
#include <pastel/sys/set/interval_set.h>
#include <pastel/sys/locator/pointerrange_locator.h>

#include <armadillo>

namespace Pastel
{

	//! Constructs a point-set from the columns of a matrix.
	/*
	Preconditions:
	The matrix is stored in column-major order.

	Optional arguments:

	dBegin, dEnd (0 <= integer <= set.n_rows):
	The range of rows which use as the coordinates 
	of the points. 
	Default: 0, set.n_rows.
	*/
	template <
		typename Real,
		typename... ArgumentSet>
	decltype(auto) matrixPointSet(
		const arma::Mat<Real>& set,
		ArgumentSet&&... argumentSet)
	{
		const integer dBegin = PASTEL_ARG_S(dBegin, 0);
		const integer dEnd = PASTEL_ARG_S(dEnd, set.n_rows);

		ENSURE_OP(dBegin, >=, 0);
		ENSURE_OP(dBegin, <, dEnd);
		ENSURE_OP(dEnd, <=, set.n_rows);

		return Pastel::locationSet(
			Pastel::sparseSet(
				Pastel::intervalSet(set.memptr(), set.memptr() + set.size()), 
				set.n_rows), 
			Pastel::PointerRange_Locator<real>(dBegin, dEnd));
	}

}

#endif
