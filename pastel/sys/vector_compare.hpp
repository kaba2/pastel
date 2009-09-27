#ifndef PASTEL_VECTOR_COMPARE_HPP
#define PASTEL_VECTOR_COMPARE_HPP

#include "pastel/sys/vector_compare.h"
#include "pastel/sys/tuple_tools.h"

namespace Pastel
{

	// Comparison functions

	template <typename Real, int N>
	inline bool allEqual(
		const Vector<Real, N>& that)
	{
		return Pastel::allEqual(asTuple(that));
	}

	template <typename Real, int N>
	inline bool allEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right)
	{
		return Pastel::allEqual(asTuple(left), asTuple(right));
	}

	template <typename Real, int N>
	inline bool allEqual(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::allEqual(asTuple(left), right);
	}

	template <typename Real, int N>
	inline bool allEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right)
	{
		return Pastel::allEqual(left, asTuple(right));
	}

	template <typename Real, int N>
	inline bool anyEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right)
	{
		return Pastel::anyEqual(asTuple(left), asTuple(right));
	}

	template <typename Real, int N>
	inline bool anyEqual(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::anyEqual(asTuple(left), right);
	}

	template <typename Real, int N>
	inline bool anyEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right)
	{
		return Pastel::anyEqual(left, asTuple(right));
	}

	template <typename Real, int N>
	inline bool anyLess(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right)
	{
		return Pastel::anyLess(asTuple(left), asTuple(right));
	}

	template <typename Real, int N>
	inline bool anyLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right)
	{
		return Pastel::anyLess(left, asTuple(right));
	}

	template <typename Real, int N>
	inline bool anyLess(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::anyLess(asTuple(left), right);
	}

	template <typename Real, int N>
	inline bool anyGreater(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right)
	{
		return anyLess(right, left);
	}

	template <typename Real, int N>
	inline bool anyGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right)
	{
		return anyLess(right, left);
	}

	template <typename Real, int N>
	inline bool anyGreater(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return anyLess(right, left);
	}

	template <typename Real, int N>
	inline bool anyLessEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right)
	{
		return !(allLess(right, left));
	}

	template <typename Real, int N>
	inline bool anyLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right)
	{
		return !(allLess(right, left));
	}

	template <typename Real, int N>
	inline bool anyLessEqual(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(allLess(right, left));
	}

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right)
	{
		return !(allLess(left, right));
	}

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right)
	{
		return !(allLess(left, right));
	}

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(allLess(left, right));
	}

	template <typename Real, int N>
	inline bool allLess(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right)
	{
		return Pastel::allLess(asTuple(left), asTuple(right));
	}

	template <typename Real, int N>
	inline bool allLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right)
	{
		return Pastel::allLess(left, asTuple(right));
	}

	template <typename Real, int N>
	inline bool allLess(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::allLess(asTuple(left), right);
	}

	template <typename Real, int N>
	inline bool allGreater(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right)
	{
		return allLess(right, left);
	}

	template <typename Real, int N>
	inline bool allGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right)
	{
		return allLess(right, left);
	}

	template <typename Real, int N>
	inline bool allGreater(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return allLess(right, left);
	}

	template <typename Real, int N>
	inline bool allLessEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right)
	{
		return !(anyLess(right, left));
	}

	template <typename Real, int N>
	inline bool allLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right)
	{
		return !(anyLess(right, left));
	}

	template <typename Real, int N>
	inline bool allLessEqual(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(anyLess(right, left));
	}

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right)
	{
		return !(anyLess(left, right));
	}

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<Real, N>& right)
	{
		return !(anyLess(left, right));
	}

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const Vector<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(anyLess(left, right));
	}

}

#endif
