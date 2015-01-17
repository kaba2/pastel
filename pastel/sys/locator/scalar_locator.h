// Description: Scalar locator
// Documentation: locators.txt

#ifndef PASTELSYS_SCALAR_LOCATOR_H
#define PASTELSYS_SCALAR_LOCATOR_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/point/point_concept.h"

#include <type_traits>

namespace Pastel
{

	template <typename Real_>
	class Scalar_Locator
	{
	public:
		PASTEL_STATIC_ASSERT(std::is_arithmetic<Real_>::value);
		
		using Real = Real_;
		using Point = Real_;
		static PASTEL_CONSTEXPR integer N = Dynamic;

		explicit Scalar_Locator(integer n = 1)
		: n_(n)
		{
			ENSURE_OP(n, >=, 0);
		}

		void swap(Scalar_Locator& that)
		{
			std::swap(n_, that.n_);
		}

		const Real& operator()(const Real& point, integer i) const
		{
			return point;
		}

		integer n() const
		{
			return n_;
		}

	private:
		integer n_;
	};

	template <typename Point>
	class Default_Locator<Point, EnableIf<std::is_arithmetic<Point>, void>>
	{
	public:
		Scalar_Locator<Point> operator()(
			const Point&) const
		{
			return Scalar_Locator<Point>();
		}
	};

	template <typename Real>
	auto scalarPoint(const Real& point, integer n = 1)
		-> decltype(location(point, Scalar_Locator<Real>(n)))
	{
		return location(
			point,
			Scalar_Locator<Real>(n));
	}

}

#endif
