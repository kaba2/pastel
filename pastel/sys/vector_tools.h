// Description: Algorithms for Vectors

#ifndef PASTEL_VECTOR_TOOLS_H
#define PASTEL_VECTOR_TOOLS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/tuple_tools.h"

#include <boost/utility/enable_if.hpp>

#include <vector>
#include <iostream>

namespace Pastel
{

	template <int N, typename Real, typename Expression>
	std::ostream& operator<<(std::ostream& stream,
		const VectorExpression<Real, N, Expression>& vector);

	template <int N, typename Real>
	std::istream& operator>>(std::istream& stream,
		Vector<Real, N>& vector);

	//! Returns the sum of elements.

	template <int N, typename Real, typename Expression>
	inline Real sum(const VectorExpression<Real, N, Expression>& x);

	//! Returns the product of elements.

	template <int N, typename Real, typename Expression>
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

	template <int N, typename Real, typename Expression>
	inline Vector<Real, PASTEL_ADD_N(N, -1)> shrink(
		const VectorExpression<Real, N, Expression>& that);

	template <int N, typename Real, typename Expression>
	inline Vector<Real, PASTEL_ADD_N(N, -1)> shrink(
		const VectorExpression<Real, N, Expression>& that,
		integer index);

	template <
		typename Real,
		int N,
		typename Expression>
	class VectorExtend;

	//! Returns an N + 1 vector appended from the left.

	template <int N, typename Real, typename Expression>
	inline VectorExtend<Real, N, Expression> extend(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const VectorExpression<Real, N, Expression>& right);

	//! Returns an N + 1 vector appended from the right.

	template <int N, typename Real, typename Expression>
	inline VectorExtend<Real, N, Expression> extend(
		const VectorExpression<Real, N, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns an N + 1 vector appended from a given position.

	template <int N, typename Real, typename Expression>
	inline VectorExtend<Real, N, Expression> extend(
		const VectorExpression<Real, N, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right,
		integer index);

	//! Returns the dot product of a vector with itself.
	/*!
	The dot product is given by:
	dot(that) = sum(that[i] * that[i])
	*/

	template <int N, typename Real,
		typename Expression>
		inline Real dot(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns the dot product between vectors.
	/*!
	The dot product is given by:
	dot(left, right) = sum(left[i] * right[i])
	*/

	template <int N, typename Real,
		typename LeftExpression, typename RightExpression>
		inline Real dot(
		const VectorExpression<Real, N, LeftExpression>& left,
		const VectorExpression<Real, N, RightExpression>& right);

	//! Returns the Euclidean norm of a vector.
	/*!
	The Euclidean (L2) norm is:
	norm_2(that) := sqrt(sum(that[i]^2))
	*/

	template <int N, typename Real, typename Expression>
	typename boost::enable_if_c<(N > 1 || N == Dynamic), Real>::type
		norm(const VectorExpression<Real, N, Expression>& that);

	//! Returns the Euclidean (L2) norm of a vector.
	/*!
	The Euclidean (L2) norm is:
	norm_2(that) := sqrt(sum(that[i]^2))
	*/

	template <int N, typename Real, typename Expression>
	typename boost::enable_if_c<(N == 1), Real>::type
		norm(const VectorExpression<Real, 1, Expression>& that);

	//! Returns the Manhattan (L1) norm of a vector.
	/*!
	The Manhattan (L1) norm is:
	norm_1(that) := sum(mabs(that[i]))
	*/

	template <int N, typename Real, typename Expression>
	inline Real normManhattan(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns the p:th-power of the Lp norm of a vector.
	/*!
	powerSum(that) := sum(mabs(that[i])^p)
	*/

	template <int N, typename Real, typename Expression>
	inline Real powerSum(
		const VectorExpression<Real, N, Expression>& that,
		const PASTEL_NO_DEDUCTION(Real)& metric);

	//! Returns the Lp norm of a vector.
	/*!
	Preconditions:
	metric >= 1

	The Lp norm is:
	norm_p(that) := powerSum(that)^(1/p)

	Note: the function norm_p is not a norm
	for p e ]0, 1[.
	*/

	template <int N, typename Real, typename Expression>
	inline Real pNorm(
		const VectorExpression<Real, N, Expression>& that,
		const PASTEL_NO_DEDUCTION(Real)& metric);

	//! Returns the infinity norm of a vector.
	/*!
	The infinity norm is
	norm_inf(that) := max(mabs(that[i])).
	*/

	template <int N, typename Real, typename Expression>
	inline Real normInfinity(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns the corresponding unit vector (Euclidean norm).
	/*!
	Preconditions:
	norm(that) > 0
	*/
	template <int N, typename Real>
	inline Vector<Real, N> normalize(
		const Vector<Real, N>& that);

	//! Returns a clockwise perpendicular to the given vector in 2D.

	template <typename Real, typename Expression>
	Vector<Real, 2> cross(
		const VectorExpression<Real, 2, Expression>& that);

	//! Returns the cross product of two vectors in 3D.

	template <typename Real, typename ExpressionX,
	typename ExpressionY>
	Vector<Real, 3> cross(
		const VectorExpression<Real, 3, ExpressionX>& x,
		const VectorExpression<Real, 3, ExpressionY>& y);

}

#include "pastel/sys/vector_tools_more.h"
#include "pastel/sys/vector_compare.h"

#include "pastel/sys/vector_tools.hpp"
#include "pastel/sys/vector_tools_more.hpp"

#endif
