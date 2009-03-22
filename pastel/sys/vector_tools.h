/*!
\file
\brief Functions for working with vectors.
*/

#ifndef PASTELSYS_VECTOR_TOOLS_H
#define PASTELSYS_VECTOR_TOOLS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/tuple_tools.h"

#include <boost/utility/enable_if.hpp>

#include <vector>
#include <iostream>

namespace Pastel
{

	template <int N, typename Real>
	std::ostream& operator<<(std::ostream& stream,
		const Vector<N, Real>& vector);

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

	template <int N, typename Real>
	inline Vector<N, Real> unitAxis(integer index);

	//! Returns a subsequence of a vector.

	template <int N, typename Real, typename Expression>
	inline Vector<N - 1, Real> shrink(
		const VectorExpression<N, Real, Expression>& that);

	template <int N, typename Real, typename Expression>
	inline Vector<N - 1, Real> shrink(
		const VectorExpression<N, Real, Expression>& that,
		integer index);

	//! Returns an N + 1 vector appended from the left.

	template <int N, typename Real, typename Expression>
	inline Vector<N + 1, Real> extend(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const VectorExpression<N, Real, Expression>& right);

	//! Returns an N + 1 vector appended from the right.

	template <int N, typename Real, typename Expression>
	inline Vector<N + 1, Real> extend(
		const VectorExpression<N, Real, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	template <int N, typename Real, typename Expression>
	inline Vector<N + 1, Real> extend(
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

	template <int N, typename Real>
	typename boost::enable_if_c<(N > 1), Real>::type
		norm(const Vector<N, Real>& that);

	//! Returns the Euclidean (L2) norm of a vector.
	/*!
	The Euclidean (L2) norm is:
	norm_2(that) := sqrt(sum(that[i]^2))
	*/

	template <int N, typename Real>
	typename boost::enable_if_c<(N == 1), Real>::type
		norm(const Vector<1, Real>& that);

	//! Returns the Manhattan (L1) norm of a vector.
	/*!
	The Manhattan (L1) norm is:
	norm_1(that) := sum(abs(that[i]))
	*/

	template <int N, typename Real>
	inline Real normManhattan(const Vector<N, Real>& that);

	//! Returns the p:th-power of the Lp norm of a vector.
	/*!
	powerSum(that) := sum(abs(that[i])^p)
	*/

	template <int N, typename Real>
	inline Real powerSum(const Vector<N, Real>& that,
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

	template <int N, typename Real>
	inline Real pNorm(const Vector<N, Real>& that,
		const PASTEL_NO_DEDUCTION(Real)& metric);

	//! Returns the infinity norm of a vector.
	/*!
	The infinity norm is
	norm_inf(that) := max(abs(that[i])).
	*/

	template <int N, typename Real>
	inline Real normInfinity(const Vector<N, Real>& that);

	//! Returns the corresponding unit vector (Euclidean norm).
	/*!
	Preconditions:
	norm(that) > 0
	*/
	template <int N, typename Real>
	inline Vector<N, Real> normalize(const Vector<N, Real>& that);

	//! Returns a clockwise perpendicular to the given vector.

	template <typename Real>
	inline Vector<2, Real> cross(const Vector<2, Real>& that);

	//! Returns the cross product of two vectors.

	template <typename Real>
	inline Vector<3, Real> cross(
		const Vector<3, Real>& x,
		const Vector<3, Real>& y);

	// Comparison functions

	//! Returns 'for all i: that[i] == that[0]'.

	template <int N, typename Real>
	inline bool allEqual(
		const Vector<N, Real>& that);

	//! Returns 'for all i: left[i] == right[i]'.

	template <int N, typename Real>
	inline bool allEqual(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right);

	//! Returns 'for all i: left[i] == right'.

	template <int N, typename Real>
	inline bool allEqual(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left == right[i]'.

	template <int N, typename Real>
	inline bool allEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right);

	//! Returns 'exists i: left[i] == right[i]'.

	template <int N, typename Real>
	inline bool anyEqual(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right);

	//! Returns 'exists i: left[i] == right'.

	template <int N, typename Real>
	inline bool anyEqual(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left == right[i]'.

	template <int N, typename Real>
	inline bool anyEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right);

	//! Returns 'exists i: left[i] < right[i]'.

	template <int N, typename Real>
	inline bool anyLess(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right);

	//! Returns 'exists i: left < right[i]'.

	template <int N, typename Real>
	inline bool anyLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right);

	//! Returns 'exists i: left[i] < right'.

	template <int N, typename Real>
	inline bool anyLess(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left[i] > right[i]'.

	template <int N, typename Real>
	inline bool anyGreater(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right);

	//! Returns 'exists i: left > right[i]'.

	template <int N, typename Real>
	inline bool anyGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right);

	//! Returns 'exists i: left[i] > right'.

	template <int N, typename Real>
	inline bool anyGreater(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left[i] <= right[i]'.

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right);

	//! Returns 'exists i: left <= right[i]'.

	template <int N, typename Real>
	inline bool anyLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right);

	//! Returns 'exists i: left[i] <= right'.

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left[i] >= right[i]'.

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right);

	//! Returns 'exists i: left >= right[i]'.

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right);

	//! Returns 'exists i: left[i] >= right'.

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] < right[i]'.

	template <int N, typename Real>
	inline bool allLess(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right);

	//! Returns 'for all i: left < right[i]'.

	template <int N, typename Real>
	inline bool allLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right);

	//! Returns 'for all i: left[i] < right'.

	template <int N, typename Real>
	inline bool allLess(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] > right[i]'.

	template <int N, typename Real>
	inline bool allGreater(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right);

	//! Returns 'for all i: left > right[i]'.

	template <int N, typename Real>
	inline bool allGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right);

	//! Returns 'for all i: left[i] > right'.

	template <int N, typename Real>
	inline bool allGreater(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] <= right[i]'.

	template <int N, typename Real>
	inline bool allLessEqual(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right);

	//! Returns 'for all i: left <= right[i]'.

	template <int N, typename Real>
	inline bool allLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right);

	//! Returns 'for all i: left[i] <= right'.

	template <int N, typename Real>
	inline bool allLessEqual(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] >= right[i]'.

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right);

	//! Returns 'for all i: left >= right[i]'.

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right);

	//! Returns 'for all i: left[i] >= right'.

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

}

#include "pastel/sys/vector_tools_more.h"

#include "pastel/sys/vector_tools.hpp"
#include "pastel/sys/vector_tools_more.hpp"

#endif
