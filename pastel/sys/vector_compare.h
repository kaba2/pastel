// Description: Array comparison for Vectors
// Detail: allLess, anyLess, allEqual, anyEqual, etc.

#ifndef PASTELSYS_VECTOR_COMPARE_H
#define PASTELSYS_VECTOR_COMPARE_H

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
		const NoDeduction<Real>& right);

	//! Returns 'for all i: left == right[i]'.

	template <typename Real, int N>
	inline bool allEqual(
		const NoDeduction<Real>& left,
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
		const NoDeduction<Real>& right);

	//! Returns 'exists i: left == right[i]'.

	template <typename Real, int N>
	inline bool anyEqual(
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left[i] < right[i]'.

	template <typename Real, int N>
	inline bool anyLess(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left < right[i]'.

	template <typename Real, int N>
	inline bool anyLess(
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left[i] < right'.

	template <typename Real, int N>
	inline bool anyLess(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right);

	//! Returns 'exists i: left[i] > right[i]'.

	template <typename Real, int N>
	inline bool anyGreater(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left > right[i]'.

	template <typename Real, int N>
	inline bool anyGreater(
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left[i] > right'.

	template <typename Real, int N>
	inline bool anyGreater(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right);

	//! Returns 'exists i: left[i] <= right[i]'.

	template <typename Real, int N>
	inline bool anyLessEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left <= right[i]'.

	template <typename Real, int N>
	inline bool anyLessEqual(
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left[i] <= right'.

	template <typename Real, int N>
	inline bool anyLessEqual(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right);

	//! Returns 'exists i: left[i] >= right[i]'.

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left >= right[i]'.

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right);

	//! Returns 'exists i: left[i] >= right'.

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right);

	//! Returns 'for all i: left[i] < right[i]'.

	template <typename Real, int N>
	inline bool allLess(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left < right[i]'.

	template <typename Real, int N>
	inline bool allLess(
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left[i] < right'.

	template <typename Real, int N>
	inline bool allLess(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right);

	//! Returns 'for all i: left[i] > right[i]'.

	template <typename Real, int N>
	inline bool allGreater(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left > right[i]'.

	template <typename Real, int N>
	inline bool allGreater(
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left[i] > right'.

	template <typename Real, int N>
	inline bool allGreater(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right);

	//! Returns 'for all i: left[i] <= right[i]'.

	template <typename Real, int N>
	inline bool allLessEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left <= right[i]'.

	template <typename Real, int N>
	inline bool allLessEqual(
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left[i] <= right'.

	template <typename Real, int N>
	inline bool allLessEqual(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right);

	//! Returns 'for all i: left[i] >= right[i]'.

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left >= right[i]'.

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right);

	//! Returns 'for all i: left[i] >= right'.

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right);

}

#include "pastel/sys/vector_compare.hpp"

#endif
