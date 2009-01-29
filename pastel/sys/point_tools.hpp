#ifndef PASTELSYS_POINT_TOOLS_HPP
#define PASTELSYS_POINT_TOOLS_HPP

#include "pastel/sys/point_tools.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	std::ostream& operator<<(std::ostream& stream,
		const Point<N, Real>& point)
	{
		for (integer i = 0;i < N;++i)
		{
			stream << point[i] << " ";
		}

		return stream;
	}

	template <int N, typename Real>
	std::istream& operator>>(std::istream& stream,
		Point<N, Real>& point)
	{
		for (integer i = 0;i < N;++i)
		{
			stream >> point[i];
		}

		return stream;
	}

	template <int N, typename Real>
	integer minIndex(
		const Point<N, Real>& that)
	{
		return Pastel::minIndex(asVector(that));
	}

	template <int N, typename Real>
	Point<N, Real> min(
		const Point<N, Real>& left,
		const Point<N, Real>& right)
	{
		return asPoint(Pastel::min(asVector(left), asVector(right)));
	}

	template <int N, typename Real>
	Point<N, Real> min(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right)
	{
		return asPoint(Pastel::min(left, asVector(right)));
	}

	template <int N, typename Real>
	Point<N, Real> min(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return asPoint(Pastel::min(asVector(left), right));
	}

	template <int N, typename Real>
	integer maxIndex(
		const Point<N, Real>& that)
	{
		return Pastel::maxIndex(asVector(that));
	}

	template <int N, typename Real>
	Point<N, Real> max(
		const Point<N, Real>& left,
		const Point<N, Real>& right)
	{
		return asPoint(Pastel::max(asVector(left), asVector(right)));
	}

	template <int N, typename Real>
	Point<N, Real> max(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right)
	{
		return asPoint(Pastel::max(left, asVector(right)));
	}

	template <int N, typename Real>
	Point<N, Real> max(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return asPoint(Pastel::max(asVector(left), right));
	}

	template <int N, typename Real>
	Point<N - 1, Real> shrink(
		const Point<N, Real>& that)
	{
		return asPoint(Pastel::shrink(asVector(that)));
	}

	template <int N, typename Real>
	Point<N + 1, Real> extend(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right)
	{
		return asPoint(Pastel::extend(left, asVector(right)));
	}

	template <int N, typename Real>
	Point<N + 1, Real> extend(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return asPoint(Pastel::extend(asVector(left), right));
	}

	// Comparison functions

	template <int N, typename Real>
	inline bool allEqual(
		const Point<N, Real>& that)
	{
		return Pastel::allEqual(asTuple(that));
	}

	template <int N, typename Real>
	inline bool allEqual(
		const Point<N, Real>& left,
		const Point<N, Real>& right)
	{
		return Pastel::allEqual(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool allEqual(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::allEqual(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool allEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right)
	{
		return Pastel::allEqual(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyEqual(
		const Point<N, Real>& left,
		const Point<N, Real>& right)
	{
		return Pastel::anyEqual(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyEqual(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::anyEqual(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool anyEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right)
	{
		return Pastel::anyEqual(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyLess(
		const Point<N, Real>& left,
		const Point<N, Real>& right)
	{
		return Pastel::anyLess(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right)
	{
		return Pastel::anyLess(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyLess(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::anyLess(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const Point<N, Real>& left,
		const Point<N, Real>& right)
	{
		return Pastel::anyGreater(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right)
	{
		return Pastel::anyGreater(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool anyGreater(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::anyGreater(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Point<N, Real>& left,
		const Point<N, Real>& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyLessEqual(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(allLess(right, left));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Point<N, Real>& left,
		const Point<N, Real>& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool anyGreaterEqual(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(allLess(left, right));
	}

	template <int N, typename Real>
	inline bool allLess(
		const Point<N, Real>& left,
		const Point<N, Real>& right)
	{
		return Pastel::allLess(asTuple(left), asTuple(right));
	}

	template <int N, typename Real>
	inline bool allLess(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right)
	{
		return Pastel::allLess(left, asTuple(right));
	}

	template <int N, typename Real>
	inline bool allLess(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return Pastel::allLess(asTuple(left), right);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const Point<N, Real>& left,
		const Point<N, Real>& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allGreater(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return allLess(right, left);
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const Point<N, Real>& left,
		const Point<N, Real>& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allLessEqual(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(anyLess(right, left));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Point<N, Real>& left,
		const Point<N, Real>& right)
	{
		return !(anyLess(left, right));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Point<N, Real>& right)
	{
		return !(anyLess(left, right));
	}

	template <int N, typename Real>
	inline bool allGreaterEqual(
		const Point<N, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return !(anyLess(left, right));
	}

}

#endif
