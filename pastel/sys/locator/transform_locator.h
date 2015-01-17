// Description: Transform locator
// Documentation: locators.txt

#ifndef PASTELSYS_TRANSFORM_LOCATOR_H
#define PASTELSYS_TRANSFORM_LOCATOR_H

#include "pastel/sys/locator_concept.h"
#include "pastel/sys/point_concept.h"

namespace Pastel
{

	template <
		typename Point_,
		typename Locator,
		typename Transform>
	class Transform_Locator
	{
	public:
		static PASTEL_CONSTEXPR int N = Locator::N;
		using Real = typename Locator::Real;
		using Point = Point_;

		Transform_Locator(
			const Locator& locator,
			const Transform& transform)
			: locator_(locator)
			, transform_(transform)
		{
		}

		void swap(Transform_Locator& that)
		{
			locator_.swap(that.locator_);
		}

		integer n() const
		{
			return locator_.n();
		}

		const Real& operator()(
			const Point& point, integer i) const
		{
			return locator_(transform(point), i);
		}

	private:
		Locator locator_;
		Transform transform_;
	};

	template <
		typename Point,
		typename Locator,
		typename Transform>
	Transform_Locator<Point, Locator, Transform> transformLocator(
		const Locator& locator,
		const Transform& transform)
	{
		return Transform_Locator<Point, Locator, Transform>(locator, transform);
	}

}

#endif