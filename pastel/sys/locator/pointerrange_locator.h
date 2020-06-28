// Description: Pointer-range locator
// Documentation: locators.txt

#ifndef PASTELSYS_POINTERRANGE_LOCATOR_H
#define PASTELSYS_POINTERRANGE_LOCATOR_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real_, int N_ = Dynamic>
	class PointerRange_Locator
	{
	public:
		static constexpr int N = N_;
		using Real = Real_;
		using Point = const Real*;

		explicit PointerRange_Locator(
			integer begin = 0, 
			integer end = ((N > 0) ? N : 0))
			: begin_(begin)
			, end_(end)
		{
			ENSURE(N == Dynamic || end == N);
			ENSURE_OP(begin, <=, end);
		}

		integer n() const
		{
			return end_ - begin_;
		}

		integer n(const Point& point) const
		{
			return n();
		}

		const Real& operator()(Point point, integer i) const
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, n());
			return (point + begin_)[i];
		}

	private:
		integer begin_;
		integer end_;
	};

	template <typename Real_, int N_ = Dynamic>
	decltype(auto) pointerRangeLocator(integer begin = 0, integer end = N_)
	{
		return PointerRange_Locator<Real_, N_>(begin, end);
	}

	template <typename Real>
	decltype(auto) pointerRangePoint(Real* point, integer begin, integer end)
	{
		return location(
			point,
			PointerRange_Locator<Real>(begin, end));
	}

}

#endif
