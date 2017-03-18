// Description: Locator of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_LOCATOR_H
#define PASTELSYS_POINT_LOCATOR_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/point/point_real.h"
#include "pastel/sys/function/identity_function.h"
#include "pastel/sys/type_traits/remove_cvref.h"

namespace Pastel
{

	template <typename Point_, typename Real_, integer N_ = Dynamic>
	class Usual_Locator
	{
	public:
		static constexpr integer N = N_;

		using Point = Point_;
		using Real = Real_;

		explicit Usual_Locator(integer dimension = N_)
			: n_(dimension)
		{
		}

		integer n() const
		{
			return n_;
		}

		decltype(auto) operator()(
			const Point& point, integer i) const
		{
			return pointAxis(point, i);
		}

		integer n_;
	};

	template <
		typename Point,
		Requires<
			Models<Point, Point_Concept>
		> = 0
	>
	decltype(auto) pointLocator(const Point& point)
	{
		return Usual_Locator<Point, Point_Real<Point>>(dimension(point));
	}

}

namespace Pastel
{

	template <
		typename Point,
		Requires<
			Models<Point, Point_Concept>
		> = 0
	>
	using Point_Locator = 
		decltype(pointLocator(std::declval<Point>()));

	template <
		typename Point,
		Requires<
			Models<Point, Point_Concept>
		> = 0
	>
	using Point_Locator_F = 
		Identity_F<Point_Locator<Point>>;

}

#endif
