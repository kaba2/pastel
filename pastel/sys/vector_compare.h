// Description: Tools to compare vectors
// Detail: allLess, anyLess, allEqual, anyEqual, etc.

#ifndef PASTEL_VECTOR_COMPARE_H
#define PASTEL_VECTOR_COMPARE_H

#include "pastel/sys/vector.h"

namespace Pastel
{

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

#include "pastel/sys/vector_compare.hpp"

#endif
