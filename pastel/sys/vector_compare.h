// Description: Array comparison for Vectors
// Detail: allLess, anyLess, allEqual, anyEqual, etc.

#ifndef PASTEL_VECTOR_COMPARE_H
#define PASTEL_VECTOR_COMPARE_H

#include "pastel/sys/vector.h"

namespace Pastel
{

	// Comparison functions

	//! Returns 'for all i: that[i] == that[0]'.

	template <typename Real, int N>
	inline bool allEqual(
		const Vector<Real, N>& that);

	//! Returns 'for all i: left[i] == right[i]'.

	template <typename Real, int N>
	inline bool allEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left[i] == right'.

	template <typename Real, int N>
	inline bool allEqual(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left == right[i]'.

	template <typename Real, int N>
	inline bool allEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left[i] == right[i]'.

	template <typename Real, int N>
	inline bool anyEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left[i] == right'.

	template <typename Real, int N>
	inline bool anyEqual(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left == right[i]'.

	template <typename Real, int N>
	inline bool anyEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left[i] < right[i]'.

	template <typename Real, int N>
	inline bool anyLess(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left < right[i]'.

	template <typename Real, int N>
	inline bool anyLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left[i] < right'.

	template <typename Real, int N>
	inline bool anyLess(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left[i] > right[i]'.

	template <typename Real, int N>
	inline bool anyGreater(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left > right[i]'.

	template <typename Real, int N>
	inline bool anyGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left[i] > right'.

	template <typename Real, int N>
	inline bool anyGreater(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left[i] <= right[i]'.

	template <typename Real, int N>
	inline bool anyLessEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left <= right[i]'.

	template <typename Real, int N>
	inline bool anyLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left[i] <= right'.

	template <typename Real, int N>
	inline bool anyLessEqual(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left[i] >= right[i]'.

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left >= right[i]'.

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left[i] >= right'.

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] < right[i]'.

	template <typename Real, int N>
	inline bool allLess(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left < right[i]'.

	template <typename Real, int N>
	inline bool allLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left[i] < right'.

	template <typename Real, int N>
	inline bool allLess(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] > right[i]'.

	template <typename Real, int N>
	inline bool allGreater(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left > right[i]'.

	template <typename Real, int N>
	inline bool allGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left[i] > right'.

	template <typename Real, int N>
	inline bool allGreater(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] <= right[i]'.

	template <typename Real, int N>
	inline bool allLessEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left <= right[i]'.

	template <typename Real, int N>
	inline bool allLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left[i] <= right'.

	template <typename Real, int N>
	inline bool allLessEqual(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] >= right[i]'.

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left >= right[i]'.

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left[i] >= right'.

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

}

#include "pastel/sys/vector_compare.hpp"

#endif
