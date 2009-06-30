/*!
\file
\brief Functions for working with vectors.
*/

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
		const VectorExpression<N, Real, Expression>& vector);

	template <int N, typename Real>
	std::istream& operator>>(std::istream& stream,
		Vector<N, Real>& vector);

	//! Returns the sum of elements.

	template <int N, typename Real, typename Expression>
	inline Real sum(const VectorExpression<N, Real, Expression>& x);

	//! Returns the product of elements.

	template <int N, typename Real, typename Expression>
	inline Real product(const VectorExpression<N, Real, Expression>& x);

	//! Returns the 'index'th natural basis axis.
	/*!
	N != Dynamic
	index >= 0 && index < N
	*/

	template <
		int N,
		typename Real>
	class VectorUnitAxis;

	template <int N, typename Real>
	inline VectorUnitAxis<N, Real> unitAxis(integer index);

	//! Returns the 'index'th natural basis axis.
	/*!
	index >= 0 && index < dimension
	dimension > 0
	*/

	template <int N, typename Real>
	inline VectorUnitAxis<N, Real> unitAxis(
		integer dimension, integer index);

	//! Returns a subsequence of a vector.

	template <int N, typename Real, typename Expression>
	inline TemporaryVector<PASTEL_ADD_N(N, -1), Real> shrink(
		const VectorExpression<N, Real, Expression>& that);

	template <int N, typename Real, typename Expression>
	inline TemporaryVector<PASTEL_ADD_N(N, -1), Real> shrink(
		const VectorExpression<N, Real, Expression>& that,
		integer index);

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorExtend;

	//! Returns an N + 1 vector appended from the left.

	template <int N, typename Real, typename Expression>
	inline VectorExtend<N, Real, Expression> extend(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const VectorExpression<N, Real, Expression>& right);

	//! Returns an N + 1 vector appended from the right.

	template <int N, typename Real, typename Expression>
	inline VectorExtend<N, Real, Expression> extend(
		const VectorExpression<N, Real, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns an N + 1 vector appended from a given position.

	template <int N, typename Real, typename Expression>
	inline VectorExtend<N, Real, Expression> extend(
		const VectorExpression<N, Real, Expression>& left,
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
		const VectorExpression<N, Real, Expression>& that);

	//! Returns the dot product between vectors.
	/*!
	The dot product is given by:
	dot(left, right) = sum(left[i] * right[i])
	*/

	template <int N, typename Real,
		typename LeftExpression, typename RightExpression>
		inline Real dot(
		const VectorExpression<N, Real, LeftExpression>& left,
		const VectorExpression<N, Real, RightExpression>& right);

	//! Returns the Euclidean norm of a vector.
	/*!
	The Euclidean (L2) norm is:
	norm_2(that) := sqrt(sum(that[i]^2))
	*/

	template <int N, typename Real, typename Expression>
	typename boost::enable_if_c<(N > 1 || N == Dynamic), Real>::type
		norm(const VectorExpression<N, Real, Expression>& that);

	//! Returns the Euclidean (L2) norm of a vector.
	/*!
	The Euclidean (L2) norm is:
	norm_2(that) := sqrt(sum(that[i]^2))
	*/

	template <int N, typename Real, typename Expression>
	typename boost::enable_if_c<(N == 1), Real>::type
		norm(const VectorExpression<1, Real, Expression>& that);

	//! Returns the Manhattan (L1) norm of a vector.
	/*!
	The Manhattan (L1) norm is:
	norm_1(that) := sum(mabs(that[i]))
	*/

	template <int N, typename Real, typename Expression>
	inline Real normManhattan(
		const VectorExpression<N, Real, Expression>& that);

	//! Returns the p:th-power of the Lp norm of a vector.
	/*!
	powerSum(that) := sum(mabs(that[i])^p)
	*/

	template <int N, typename Real, typename Expression>
	inline Real powerSum(
		const VectorExpression<N, Real, Expression>& that,
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
		const VectorExpression<N, Real, Expression>& that,
		const PASTEL_NO_DEDUCTION(Real)& metric);

	//! Returns the infinity norm of a vector.
	/*!
	The infinity norm is
	norm_inf(that) := max(mabs(that[i])).
	*/

	template <int N, typename Real, typename Expression>
	inline Real normInfinity(
		const VectorExpression<N, Real, Expression>& that);

	//! Returns the corresponding unit vector (Euclidean norm).
	/*!
	Preconditions:
	norm(that) > 0

	This version does the normalization in-place.
	*/
	template <int N, typename Real>
	inline TemporaryVector<N, Real> normalize(
		const TemporaryVector<N, Real>& that);

	//! Returns the corresponding unit vector (Euclidean norm).
	/*!
	Preconditions:
	norm(that) > 0
	*/
	template <int N, typename Real>
	inline TemporaryVector<N, Real> normalize(
		const Vector<N, Real>& that);

	//! Returns a clockwise perpendicular to the given vector in 2D.

	template <typename Real, typename Expression>
	TemporaryVector<2, Real> cross(
		const VectorExpression<2, Real, Expression>& that);

	//! Returns the cross product of two vectors in 3D.

	template <typename Real, typename ExpressionX,
	typename ExpressionY>
	TemporaryVector<3, Real> cross(
		const VectorExpression<3, Real, ExpressionX>& x,
		const VectorExpression<3, Real, ExpressionY>& y);

}

#include "pastel/sys/vector_tools_more.h"
#include "pastel/sys/vector_compare.h"

#include "pastel/sys/vector_tools.hpp"
#include "pastel/sys/vector_tools_more.hpp"

#endif
