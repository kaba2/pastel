#ifndef PASTEL_VECTOR_COMPARE_HPP
#define PASTEL_VECTOR_COMPARE_HPP

#include "pastel/sys/vector_compare.h"
#include "pastel/sys/tuple_tools.h"

namespace Pastel
{

	// Comparison functions

	template <int N, typename Real>
	inline bool allEqual(
		const Vector<N, Real>& that)
	{
		return Pastel::allEqual(asTuple(that));
	}

	template <int N, typename Real>
	inline bool allEqual(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right)
	{
		return Pastel::allEqual(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool allEqual(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::allEqual(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool allEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right)
	{
		return Pastel::allEqual(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyEqual(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right)
	{
		return Pastel::anyEqual(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyEqual(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::anyEqual(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool anyEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right)
	{
		return Pastel::anyEqual(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyLess(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right)
	{
		return Pastel::anyLess(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right)
	{
		return Pastel::anyLess(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyLess(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::anyLess(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right)
	{
		return anyLess(right, left);
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right)
	{
		return anyLess(right, left);
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return anyLess(right, left);
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool allLess(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right)
	{
		return Pastel::allLess(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool allLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right)
	{
		return Pastel::allLess(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool allLess(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::allLess(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Vector<N, Real>& left,
		const Vector<N, Real>& right)
	{
		return !(anyLess(left, right));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Vector<N, Real>& right)
	{
		return !(anyLess(left, right));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Vector<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(anyLess(left, right));
	}

}

#endif
