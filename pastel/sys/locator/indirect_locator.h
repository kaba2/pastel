// Description: Indirect locator
// Documentation: locators.txt

#ifndef PASTELSYS_INDIRECT_LOCATOR_H
#define PASTELSYS_INDIRECT_LOCATOR_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/point/point_concept.h"

namespace Pastel
{

	template <
		typename Point_,
		typename Locator>
	class Indirect_Locator
	{
	public:
		static constexpr integer N = Locator::N;
		using Real = typename Locator::Real;
		using Point = Point_;

		explicit Indirect_Locator(
			const Locator& locator)
			: locator_(locator)
		{
		}

		integer n() const
		{
			return locator_.n();
		}

		integer n(const Point& point) const
		{
			return locator_.n(*point);
		}

		const Real& operator()(
			const Point& point, integer i) const
		{
			return locator_(*point, i);
		}

	private:
		Locator locator_;
	};

	template <
		typename Point,
		typename Locator>
	Indirect_Locator<Point, Locator> indirectLocator(
		const Locator& locator)
	{
		return Indirect_Locator<Point, Locator>(locator);
	}

}

#endif
