/*!
\file
\brief Functions for dealing with points.
*/

#ifndef PASTELSYS_POINT_TOOLS_H
#define PASTELSYS_POINT_TOOLS_H

#include "pastel/sys/point.h"
#include "pastel/sys/tuple_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	std::ostream& operator<<(std::ostream& stream,
		const Point<N, Real>& point);

	template <int N, typename Real>
	std::istream& operator>>(std::istream& stream,
		Point<N, Real>& point);

	// Parallel functions

	//! Returns the index of the minimum element.

	template <int N, typename Real>
	inline integer minIndex(
		const Point<N, Real>& that);

    template <int N, typename Real>
	Point<N, Real> min(
		const Point<N, Real>& left,
		const Point<N, Real>& right);

	template <int N, typename Real>
	Point<N, Real> min(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right);

	template <int N, typename Real>
	Point<N, Real> min(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns the index of the maximum element.

	template <int N, typename Real>
	inline integer maxIndex(
		const Point<N, Real>& that);

    template <int N, typename Real>
	Point<N, Real> max(
		const Point<N, Real>& left,
		const Point<N, Real>& right);

	template <int N, typename Real>
	Point<N, Real> max(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right);

	template <int N, typename Real>
	Point<N, Real> max(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	template <int N, typename Real>
	Point<N - 1, Real> shrink(
		const Point<N, Real>& that);

	template <int N, typename Real>
	Point<N + 1, Real> extend(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right);

	template <int N, typename Real>
	Point<N + 1, Real> extend(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	// Comparison functions
	
	//! Returns 'for all i: that[i] == that[0]'.

	template <int N, typename Real>
	inline bool allEqual(
		const Point<N, Real>& that);

	//! Returns 'for all i: left[i] == right[i]'.

	template <int N, typename Real>
	inline bool allEqual(
		const Point<N, Real>& left, 
		const Point<N, Real>& right);

	//! Returns 'for all i: left[i] == right'.

	template <int N, typename Real>
	inline bool allEqual(
		const Point<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left == right[i]'.

	template <int N, typename Real>
	inline bool allEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right);

	//! Returns 'exists i: left[i] == right[i]'.

	template <int N, typename Real>
	inline bool anyEqual(
		const Point<N, Real>& left, 
		const Point<N, Real>& right);

	//! Returns 'exists i: left[i] == right'.

	template <int N, typename Real>
	inline bool anyEqual(
		const Point<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left == right[i]'.

	template <int N, typename Real>
	inline bool anyEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right);

	//! Returns 'exists i: left[i] < right[i]'.

	template <int N, typename Real>
	inline bool anyLess(
		const Point<N, Real>& left, 
		const Point<N, Real>& right);

	//! Returns 'exists i: left < right[i]'.

	template <int N, typename Real>
	inline bool anyLess(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Point<N, Real>& right);

	//! Returns 'exists i: left[i] < right'.

	template <int N, typename Real>
	inline bool anyLess(
		const Point<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left[i] > right[i]'.

	template <int N, typename Real>
	inline bool anyGreater(
		const Point<N, Real>& left, 
		const Point<N, Real>& right);

	//! Returns 'exists i: left > right[i]'.

	template <int N, typename Real>
	inline bool anyGreater(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Point<N, Real>& right);

	//! Returns 'exists i: left[i] > right'.

	template <int N, typename Real>
	inline bool anyGreater(
		const Point<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left[i] <= right[i]'.

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Point<N, Real>& left, 
		const Point<N, Real>& right);

	//! Returns 'exists i: left <= right[i]'.

	template <int N, typename Real>
	inline bool anyLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Point<N, Real>& right);

	//! Returns 'exists i: left[i] <= right'.

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Point<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left[i] >= right[i]'.

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Point<N, Real>& left, 
		const Point<N, Real>& right);

	//! Returns 'exists i: left >= right[i]'.

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Point<N, Real>& right);

	//! Returns 'exists i: left[i] >= right'.

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Point<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] < right[i]'.

	template <int N, typename Real>
	inline bool allLess(
		const Point<N, Real>& left, 
		const Point<N, Real>& right);

	//! Returns 'for all i: left < right[i]'.

	template <int N, typename Real>
	inline bool allLess(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Point<N, Real>& right);

	//! Returns 'for all i: left[i] < right'.

	template <int N, typename Real>
	inline bool allLess(
		const Point<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] > right[i]'.

	template <int N, typename Real>
	inline bool allGreater(
		const Point<N, Real>& left, 
		const Point<N, Real>& right);

	//! Returns 'for all i: left > right[i]'.

	template <int N, typename Real>
	inline bool allGreater(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Point<N, Real>& right);

	//! Returns 'for all i: left[i] > right'.

	template <int N, typename Real>
	inline bool allGreater(
		const Point<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] <= right[i]'.

	template <int N, typename Real>
	inline bool allLessEqual(
		const Point<N, Real>& left, 
		const Point<N, Real>& right);

	//! Returns 'for all i: left <= right[i]'.

	template <int N, typename Real>
	inline bool allLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Point<N, Real>& right);

	//! Returns 'for all i: left[i] <= right'.

	template <int N, typename Real>
	inline bool allLessEqual(
		const Point<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] >= right[i]'.

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Point<N, Real>& left, 
		const Point<N, Real>& right);

	//! Returns 'for all i: left >= right[i]'.

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left, 
		const Point<N, Real>& right);

	//! Returns 'for all i: left[i] >= right'.

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Point<N, Real>& left, 
		const PASTEL_NO_DEDUCTION(Real)& right);

}

#include "pastel/sys/point_tools.hpp"

#endif
