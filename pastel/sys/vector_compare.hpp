#ifndef PASTELSYS_VECTOR_COMPARE_HPP
#define PASTELSYS_VECTOR_COMPARE_HPP

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
		const NoDeduction<Real>& right)
	{
		return Pastel::allEqual(asTuple(left), right);
	}

	template <typename Real, int N>
	inline bool allEqual(
		const NoDeduction<Real>& left,
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
		const NoDeduction<Real>& right)
	{
		return Pastel::anyEqual(asTuple(left), right);
	}

	template <typename Real, int N>
	inline bool anyEqual(
		const NoDeduction<Real>& left,
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
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right)
	{
		return Pastel::anyLess(left, asTuple(right));
	}

	template <typename Real, int N>
	inline bool anyLess(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right)
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
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right)
	{
		return anyLess(right, left);
	}

	template <typename Real, int N>
	inline bool anyGreater(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right)
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
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right)
	{
		return !(allLess(right, left));
	}

	template <typename Real, int N>
	inline bool anyLessEqual(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right)
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
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right)
	{
		return !(allLess(left, right));
	}

	template <typename Real, int N>
	inline bool anyGreaterEqual(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right)
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
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right)
	{
		return Pastel::allLess(left, asTuple(right));
	}

	template <typename Real, int N>
	inline bool allLess(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right)
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
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right)
	{
		return allLess(right, left);
	}

	template <typename Real, int N>
	inline bool allGreater(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right)
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
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right)
	{
		return !(anyLess(right, left));
	}

	template <typename Real, int N>
	inline bool allLessEqual(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right)
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
		const NoDeduction<Real>& left,
		const Vector<Real, N>& right)
	{
		return !(anyLess(left, right));
	}

	template <typename Real, int N>
	inline bool allGreaterEqual(
		const Vector<Real, N>& left,
		const NoDeduction<Real>& right)
	{
		return !(anyLess(left, right));
	}

}

#endif
