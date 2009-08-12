#ifndef PASTEL_POINT_TOOLS_HPP
#define PASTEL_POINT_TOOLS_HPP

#include "pastel/sys/point_tools.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	std::ostream& operator<<(
		std::ostream& stream,
		const Point<Real, N>& point)
	{
		const integer size = point.size();

		for (integer i = 0;i < size;++i)
		{
			stream << point[i] << " ";
		}

		return stream;
	}

	template <int N, typename Real>
	std::istream& operator>>(
		std::istream& stream,
		Point<Real, N>& point)
	{
		const integer size = point.size();

		for (integer i = 0;i < size;++i)
		{
			stream >> point[i];
		}

		return stream;
	}

	template <int N, typename Real>
	integer minIndex(
		const Point<Real, N>& that)
	{
		return Pastel::minIndex(asVector(that));
	}

	template <int N, typename Real>
	Point<Real, N> min(
		const Point<Real, N>& left,
		const Point<Real, N>& right)
	{
		return asPoint(Pastel::min(asVector(left), asVector(right)));
	}

	template <int N, typename Real>
	Point<Real, N> min(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right)
	{
		return asPoint(Pastel::min(left, asVector(right)));
	}

	template <int N, typename Real>
	Point<Real, N> min(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return asPoint(Pastel::min(asVector(left), right));
	}

	template <int N, typename Real>
	integer maxIndex(
		const Point<Real, N>& that)
	{
		return Pastel::maxIndex(asVector(that));
	}

	template <int N, typename Real>
	Point<Real, N> max(
		const Point<Real, N>& left,
		const Point<Real, N>& right)
	{
		return asPoint(Pastel::max(asVector(left), asVector(right)));
	}

	template <int N, typename Real>
	Point<Real, N> max(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right)
	{
		return asPoint(Pastel::max(left, asVector(right)));
	}

	template <int N, typename Real>
	Point<Real, N> max(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return asPoint(Pastel::max(asVector(left), right));
	}

	template <int N, typename Real>
	Point<Real, PASTEL_ADD_N(N, -1)> shrink(
		const Point<Real, N>& that)
	{
		return asPoint(Pastel::shrink(asVector(that)));
	}

	template <int N, typename Real>
	Point<Real, PASTEL_ADD_N(N, 1)> extend(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right)
	{
		return asPoint(Pastel::extend(left, asVector(right)));
	}

	template <int N, typename Real>
	Point<Real, PASTEL_ADD_N(N, 1)> extend(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return asPoint(Pastel::extend(asVector(left), right));
	}

	// Comparison functions

	template <int N, typename Real>
	inline bool allEqual(
		const Point<Real, N>& that)
	{
		return Pastel::allEqual(asTuple(that));
	}

	template <int N, typename Real>
	inline bool allEqual(
		const Point<Real, N>& left,
		const Point<Real, N>& right)
	{
		return Pastel::allEqual(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool allEqual(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::allEqual(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool allEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right)
	{
		return Pastel::allEqual(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyEqual(
		const Point<Real, N>& left,
		const Point<Real, N>& right)
	{
		return Pastel::anyEqual(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyEqual(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::anyEqual(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool anyEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right)
	{
		return Pastel::anyEqual(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyLess(
		const Point<Real, N>& left,
		const Point<Real, N>& right)
	{
		return Pastel::anyLess(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right)
	{
		return Pastel::anyLess(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyLess(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::anyLess(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const Point<Real, N>& left,
		const Point<Real, N>& right)
	{
		return Pastel::anyGreater(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right)
	{
		return Pastel::anyGreater(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::anyGreater(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Point<Real, N>& left,
		const Point<Real, N>& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Point<Real, N>& left,
		const Point<Real, N>& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool allLess(
		const Point<Real, N>& left,
		const Point<Real, N>& right)
	{
		return Pastel::allLess(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool allLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right)
	{
		return Pastel::allLess(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool allLess(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::allLess(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const Point<Real, N>& left,
		const Point<Real, N>& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const Point<Real, N>& left,
		const Point<Real, N>& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Point<Real, N>& left,
		const Point<Real, N>& right)
	{
		return !(anyLess(left, right));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<Real, N>& right)
	{
		return !(anyLess(left, right));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Point<Real, N>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(anyLess(left, right));
	}

}

#endif
