// Description: Tools for working with points
// Detail: min, max, comparison functions, etc.

#ifndef PASTEL_POINT_TOOLS_H
#define PASTEL_POINT_TOOLS_H

#include "pastel/sys/point.h"
#include "pastel/sys/tuple_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	std::ostream& operator<<(std::ostream& stream,
		const Point<Real, N>& point);

	template <int N, typename Real>
	std::istream& operator>>(std::istream& stream,
		Point<Real, N>& point);

	// Parallel functions

	//! Returns the index of the minimum element.

	template <int N, typename Real>
	inline integer minIndex(
		const Point<Real, N>& that);

	template <int N, typename Real>
	TemporaryPoint<Real, N> min(
		const Point<Real, N>& left,
		const Point<Real, N>& right);

	template <int N, typename Real>
	TemporaryPoint<Real, N> min(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right);

	template <int N, typename Real>
	TemporaryPoint<Real, N> min(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns the index of the maximum element.

	template <int N, typename Real>
	inline integer maxIndex(
		const Point<Real, N>& that);

	template <int N, typename Real>
	TemporaryPoint<Real, N> max(
		const Point<Real, N>& left,
		const Point<Real, N>& right);

	template <int N, typename Real>
	TemporaryPoint<Real, N> max(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right);

	template <int N, typename Real>
	TemporaryPoint<Real, N> max(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	template <int N, typename Real>
	TemporaryPoint<Real, PASTEL_ADD_N(N, -1)> shrink(
		const Point<Real, N>& that);

	template <int N, typename Real>
	TemporaryPoint<Real, PASTEL_ADD_N(N, 1)> extend(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right);

	template <int N, typename Real>
	TemporaryPoint<Real, PASTEL_ADD_N(N, 1)> extend(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	// Comparison functions

	//! Returns 'for all i: that[i] == that[0]'.

	template <int N, typename Real>
	inline bool allEqual(
		const Point<Real, N>& that);

	//! Returns 'for all i: left[i] == right[i]'.

	template <int N, typename Real>
	inline bool allEqual(
		const Point<Real, N>& left,
		const Point<Real, N>& right);

	//! Returns 'for all i: left[i] == right'.

	template <int N, typename Real>
	inline bool allEqual(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left == right[i]'.

	template <int N, typename Real>
	inline bool allEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right);

	//! Returns 'exists i: left[i] == right[i]'.

	template <int N, typename Real>
	inline bool anyEqual(
		const Point<Real, N>& left,
		const Point<Real, N>& right);

	//! Returns 'exists i: left[i] == right'.

	template <int N, typename Real>
	inline bool anyEqual(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left == right[i]'.

	template <int N, typename Real>
	inline bool anyEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right);

	//! Returns 'exists i: left[i] < right[i]'.

	template <int N, typename Real>
	inline bool anyLess(
		const Point<Real, N>& left,
		const Point<Real, N>& right);

	//! Returns 'exists i: left < right[i]'.

	template <int N, typename Real>
	inline bool anyLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right);

	//! Returns 'exists i: left[i] < right'.

	template <int N, typename Real>
	inline bool anyLess(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left[i] > right[i]'.

	template <int N, typename Real>
	inline bool anyGreater(
		const Point<Real, N>& left,
		const Point<Real, N>& right);

	//! Returns 'exists i: left > right[i]'.

	template <int N, typename Real>
	inline bool anyGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right);

	//! Returns 'exists i: left[i] > right'.

	template <int N, typename Real>
	inline bool anyGreater(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left[i] <= right[i]'.

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Point<Real, N>& left,
		const Point<Real, N>& right);

	//! Returns 'exists i: left <= right[i]'.

	template <int N, typename Real>
	inline bool anyLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right);

	//! Returns 'exists i: left[i] <= right'.

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left[i] >= right[i]'.

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Point<Real, N>& left,
		const Point<Real, N>& right);

	//! Returns 'exists i: left >= right[i]'.

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right);

	//! Returns 'exists i: left[i] >= right'.

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] < right[i]'.

	template <int N, typename Real>
	inline bool allLess(
		const Point<Real, N>& left,
		const Point<Real, N>& right);

	//! Returns 'for all i: left < right[i]'.

	template <int N, typename Real>
	inline bool allLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right);

	//! Returns 'for all i: left[i] < right'.

	template <int N, typename Real>
	inline bool allLess(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] > right[i]'.

	template <int N, typename Real>
	inline bool allGreater(
		const Point<Real, N>& left,
		const Point<Real, N>& right);

	//! Returns 'for all i: left > right[i]'.

	template <int N, typename Real>
	inline bool allGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right);

	//! Returns 'for all i: left[i] > right'.

	template <int N, typename Real>
	inline bool allGreater(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] <= right[i]'.

	template <int N, typename Real>
	inline bool allLessEqual(
		const Point<Real, N>& left,
		const Point<Real, N>& right);

	//! Returns 'for all i: left <= right[i]'.

	template <int N, typename Real>
	inline bool allLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right);

	//! Returns 'for all i: left[i] <= right'.

	template <int N, typename Real>
	inline bool allLessEqual(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] >= right[i]'.

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Point<Real, N>& left,
		const Point<Real, N>& right);

	//! Returns 'for all i: left >= right[i]'.

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right);

	//! Returns 'for all i: left[i] >= right'.

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

}

#include "pastel/sys/point_tools.hpp"

#endif
