// Description: Array programming for Tuples
// Detail: makeTuple, comparison functions

#ifndef PASTELSYS_TUPLE_TOOLS_H
#define PASTELSYS_TUPLE_TOOLS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tuple.h"

#include <ostream>

namespace Pastel
{

	template <typename Type, int N>
	std::ostream& operator<<(std::ostream& stream, const Tuple<Type, N>& that);

	template <typename Type>
	Tuple<Type, 1> makeTuple(
		const Type& a);

	template <typename Type>
	Tuple<Type, 2> makeTuple(
		const Type& a, const Type& b);

	template <typename Type>
	Tuple<Type, 3> makeTuple(
		const Type& a, const Type& b, const Type& c);

	template <typename Type>
	Tuple<Type, 4> makeTuple(
		const Type& a, const Type& b, const Type& c, const Type& d);

	// Comparison functions

	//! Returns 'for all i: that[i] == that[0]'.

	template <typename Real, int N>
	inline bool allEqual(
		const Tuple<Real, N>& that);

	//! Returns 'for all i: left[i] == right[i]'.

	template <typename Real, int N>
	inline bool allEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right);

	//! Returns 'for all i: left[i] == right'.

	template <typename Real, int N>
	inline bool allEqual(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left == right[i]'.

	template <typename Real, int N>
	inline bool allEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right);

	//! Returns 'exists i: left[i] == right[i]'.

	template <typename Real, int N>
	inline bool anyEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right);

	//! Returns 'exists i: left[i] == right'.

	template <typename Real, int N>
	inline bool anyEqual(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left == right[i]'.

	template <typename Real, int N>
	inline bool anyEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right);

	//! Returns 'exists i: left[i] < right[i]'.

	template <typename Real, int N>
	inline bool anyLess(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right);

	//! Returns 'exists i: left < right[i]'.

	template <typename Real, int N>
	inline bool anyLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right);

	//! Returns 'exists i: left[i] < right'.

	template <typename Real, int N>
	inline bool anyLess(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left[i] > right[i]'.

	template <typename Real, int N>
	inline bool anyGreater(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right);

	//! Returns 'exists i: left > right[i]'.

	template <typename Real, int N>
	inline bool anyGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right);

	//! Returns 'exists i: left[i] > right'.

	template <typename Real, int N>
	inline bool anyGreater(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left[i] <= right[i]'.

	template <typename Real, int N>
	inline bool anyLessEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right);

	//! Returns 'exists i: left <= right[i]'.

	template <typename Real, int N>
	inline bool anyLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right);

	//! Returns 'exists i: left[i] <= right'.

	template <typename Real, int N>
	inline bool anyLessEqual(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'exists i: left[i] >= right[i]'.

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right);

	//! Returns 'exists i: left >= right[i]'.

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right);

	//! Returns 'exists i: left[i] >= right'.

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] < right[i]'.

	template <typename Real, int N>
	inline bool allLess(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right);

	//! Returns 'for all i: left < right[i]'.

	template <typename Real, int N>
	inline bool allLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right);

	//! Returns 'for all i: left[i] < right'.

	template <typename Real, int N>
	inline bool allLess(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] > right[i]'.

	template <typename Real, int N>
	inline bool allGreater(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right);

	//! Returns 'for all i: left > right[i]'.

	template <typename Real, int N>
	inline bool allGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right);

	//! Returns 'for all i: left[i] > right'.

	template <typename Real, int N>
	inline bool allGreater(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] <= right[i]'.

	template <typename Real, int N>
	inline bool allLessEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right);

	//! Returns 'for all i: left <= right[i]'.

	template <typename Real, int N>
	inline bool allLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right);

	//! Returns 'for all i: left[i] <= right'.

	template <typename Real, int N>
	inline bool allLessEqual(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	//! Returns 'for all i: left[i] >= right[i]'.

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const Tuple<Real, N>& left,
		const Tuple<Real, N>& right);

	//! Returns 'for all i: left >= right[i]'.

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Tuple<Real, N>& right);

	//! Returns 'for all i: left[i] >= right'.

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const Tuple<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

}

#include "pastel/sys/tuple_tools.hpp"

#endif
