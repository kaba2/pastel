// Description: Some array programming functions for vectors
// Detail: min, minIndex, max, maxIndex, permute, clamp, ...

#ifndef PASTELSYS_VECTOR_TOOLS_MORE_H
#define PASTELSYS_VECTOR_TOOLS_MORE_H

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/vector.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tuple.h"

#include "pastel/sys/math_functions.h"

/*
Note that most of the array programming functions are found
in the files 'binary_vectorexpression.h' and
'unary_vectorexpression.h'.
*/

namespace Pastel
{

	// Permutation

	template <typename Real, int N, typename Expression>
	inline Vector<Real, N> permute(
		const VectorExpression<Real, N, Expression>& that,
		const Tuple<integer, N>& permutation);

	//! Returns the index of the minimum element.

	template <typename Real, int N, typename Expression>
	inline integer minIndex(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns the minimum of elements.

	template <typename Real, int N, typename Expression>
	inline Real min(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns the index of the maximum element.

	template <typename Real, int N, typename Expression>
	inline integer maxIndex(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns the maximum of elements.

	template <typename Real, int N, typename Expression>
	inline Real max(
		const VectorExpression<Real, N, Expression>& that);

	template <typename Real, int N,
	typename ThatExpression, typename MinExpression,
	typename MaxExpression>
	inline Vector<Real, N> clamp(
		const VectorExpression<Real, N, ThatExpression>& that,
		const VectorExpression<Real, N, MinExpression>& minimum,
		const VectorExpression<Real, N, MaxExpression>& maximum);

}

#endif
