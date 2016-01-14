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
		static constexpr integer N = Dynamic;
		using Real = typename Locator::Real;
		using Point = typename Locator::Point;

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

		void swap(Sub_Locator& that)
		{
			locator_.swap(that.locator_);
		}

		integer n() const
		{
			return iEnd_ - iBegin_;
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
