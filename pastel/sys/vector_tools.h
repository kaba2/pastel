// Description: Algorithms for Vectors

#ifndef PASTELSYS_VECTOR_TOOLS_H
#define PASTELSYS_VECTOR_TOOLS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/tuple_tools.h"

#include <vector>
#include <iostream>

namespace Pastel
{

	template <typename Real, int N, typename Expression>
	std::ostream& operator<<(std::ostream& stream,
		const VectorExpression<Real, N, Expression>& vector);

	template <typename Real, int N>
	std::istream& operator>>(std::istream& stream,
		Vector<Real, N>& vector);

	//! Returns the sum of elements.

	template <typename Real, int N, typename Expression>
	inline Real sum(const VectorExpression<Real, N, Expression>& x);

	//! Returns the product of elements.

	template <typename Real, int N, typename Expression>
	inline Real product(const VectorExpression<Real, N, Expression>& x);

	//! Returns the 'index'th natural basis axis.
	/*!
	N != Dynamic
	index >= 0 && index < N
	*/

	template <
		typename Real,
		int N>
	class VectorUnitAxis;

	template <typename Real, int N>
	inline VectorUnitAxis<Real, N> unitAxis(integer index);

	//! Returns the 'index'th natural basis axis.
	/*!
	index >= 0 && index < dimension
	dimension > 0
	*/

	template <typename Real, int N>
	inline VectorUnitAxis<Real, N> unitAxis(
		integer dimension, integer index);

	//! Returns a subsequence of a vector.

	template <typename Real, int N, typename Expression>
	inline Vector<Real, ModifyN<N, N - 1>::Result> shrink(
		const VectorExpression<Real, N, Expression>& that);

	template <typename Real, int N, typename Expression>
	inline Vector<Real, ModifyN<N, N - 1>::Result> shrink(
		const VectorExpression<Real, N, Expression>& that,
		integer index);

	template <
		typename Real,
		int N,
		typename Expression>
	class VectorExtend;

	//! Returns an N + 1 vector appended from the left.

	template <typename Real, int N, typename Expression>
	inline VectorExtend<Real, N, Expression> extend(
		const NoDeduction<Real>& left,
		const VectorExpression<Real, N, Expression>& right);

	//! Returns an N + 1 vector appended from the right.

	template <typename Real, int N, typename Expression>
	inline VectorExtend<Real, N, Expression> extend(
		const VectorExpression<Real, N, Expression>& left,
		const NoDeduction<Real>& right);

	//! Returns an N + 1 vector appended from a given position.

	template <typename Real, int N, typename Expression>
	inline VectorExtend<Real, N, Expression> extend(
		const VectorExpression<Real, N, Expression>& left,
		const NoDeduction<Real>& right,
		integer index);

	//! Returns the dot product of a vector with itself.
	/*!
	The dot product is given by:
	dot(that) = sum(that[i] * that[i])
	*/

	template <typename Real, int N,
		typename Expression>
		inline Real dot(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns the dot product between vectors.
	/*!
	The dot product is given by:
	dot(left, right) = sum(left[i] * right[i])
	*/

	template <typename Real, int LeftN, int RightN,
		typename LeftExpression, typename RightExpression>
		inline Real dot(
		const VectorExpression<Real, LeftN, LeftExpression>& left,
		const VectorExpression<Real, RightN, RightExpression>& right);

	//! Returns the norm bijection of a vector.
	template <typename Real, int N, typename Expression, 
		typename NormBijection>
	Real norm2(const VectorExpression<Real, N, Expression>& that,
		const NormBijection& normBijection);

	//! Returns the Euclidean norm of a vector.
	/*!
	The Euclidean (L2) norm is:
	norm_2(that) := sqrt(sum(that[i]^2))
	*/

	template <typename Real, int N, typename Expression>
	PASTEL_ENABLE_IF_C(N > 1 || N == Dynamic, Real)
		norm(const VectorExpression<Real, N, Expression>& that);

	//! Returns the Euclidean (L2) norm of a vector.
	/*!
	The Euclidean (L2) norm is:
	norm_2(that) := sqrt(sum(that[i]^2))
	*/

	template <typename Real, int N, typename Expression>
	PASTEL_ENABLE_IF_C(N == 1, Real)
		norm(const VectorExpression<Real, 1, Expression>& that);

	//! Returns the Manhattan (L1) norm of a vector.
	/*!
	The Manhattan (L1) norm is:
	norm_1(that) := sum(mabs(that[i]))
	*/

	template <typename Real, int N, typename Expression>
	inline Real manhattanNorm(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns the p:th-power of the Lp norm of a vector.
	/*!
	powerSum(that) := sum(mabs(that[i])^p)
	*/

	template <typename Real, int N, typename Expression>
	inline Real powerSum(
		const VectorExpression<Real, N, Expression>& that,
		const NoDeduction<Real>& metric);

	//! Returns the Lp norm of a vector.
	/*!
	Preconditions:
	metric >= 1

	The Lp norm is:
	norm_p(that) := powerSum(that)^(1/p)

	Note: the function norm_p is not a norm
	for p e ]0, 1[.
	*/

	template <typename Real, int N, typename Expression>
	inline Real minkowskiNorm(
		const VectorExpression<Real, N, Expression>& that,
		const NoDeduction<Real>& metric);

	//! Returns the max norm of a vector.
	/*!
	The max norm is
	norm_max(that) := max(mabs(that[i])).
	*/

	template <typename Real, int N, typename Expression>
	inline Real maxNorm(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns the corresponding unit vector (Euclidean norm).
	/*!
	Preconditions:
	norm(that) > 0
	*/
	template <typename Real, int N>
	inline Vector<Real, N> normalize(
		const Vector<Real, N>& that);

	template <typename Real, int N, typename Expression>
	inline Vector<Real, N> normalize(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns a clockwise perpendicular to the given vector in 2D.

	template <typename Real, int N, typename Expression>
	Vector<Real, 2> cross(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns the cross product of two vectors in 3D.

	template <typename Real, int N, typename ExpressionX,
	typename ExpressionY>
	Vector<Real, 3> cross(
		const VectorExpression<Real, N, ExpressionX>& x,
		const VectorExpression<Real, N, ExpressionY>& y);

}

#include "pastel/sys/array_vectorexpression.h"
#include "pastel/sys/unary_vectorexpression.h"
#include "pastel/sys/binary_vectorexpression.h"

#include "pastel/sys/vector_tools_more.h"

#include "pastel/sys/vector_tools.hpp"
#include "pastel/sys/vector_tools_more.hpp"

#endif
