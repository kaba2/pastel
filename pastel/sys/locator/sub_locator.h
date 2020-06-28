// Description: Sub locator
// Documentation: locators.txt

#ifndef PASTELSYS_SUB_LOCATOR_H
#define PASTELSYS_SUB_LOCATOR_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/point/point_concept.h"

namespace Pastel
{

	template <
		typename Locator>
	class Sub_Locator
	{
	public:
		static constexpr int N = Dynamic;
		using Real = typename Locator::Real;
		using Point = typename Locator::Point;

		Sub_Locator()
		: locator_()
		, iBegin_(0)
		, iEnd_(0)
		{
		}

		Sub_Locator(
			const Locator& locator,
			integer iBegin,
			integer iEnd)
			: locator_(locator)
			, iBegin_(iBegin)
			, iEnd_(iEnd)
		{
			PENSURE_OP(iBegin, >= , 0);
			PENSURE_OP(iBegin, <= , iEnd);
			PENSURE_OP(iEnd, <= , locator.n());
		}

		integer n() const
		{
			return iEnd_ - iBegin_;
		}

		integer n(const Point& point) const
		{
			return n();
		}

		const Real& operator()(
			const Point& point, integer i) const
		{
			return locator_(point, i + iBegin_);
		}

	private:
		Locator locator_;
		integer iBegin_;
		integer iEnd_;
	};

	template <typename Locator>
	decltype(auto) subLocator(
		const Locator& locator,
		integer iBegin,
		integer iEnd)
	{
		return Sub_Locator<Locator>(locator, iBegin, iEnd);
	}

}

#endif
