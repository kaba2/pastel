// Description: Scalar locator
// Documentation: locators.txt

#ifndef PASTELSYS_SCALAR_LOCATOR_H
#define PASTELSYS_SCALAR_LOCATOR_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/point/point_concept.h"

#include <type_traits>

namespace Pastel
{

	template <
		typename Real_,
		integer N_ = 1>
	class Scalar_Locator
	{
	public:
		using Real = Real_;
		using Point = Real_;
		static constexpr integer N = N_;

		explicit Scalar_Locator(integer n = 1)
		: n_(n)
		{
			ENSURE_OP(n, >=, 0);
			ENSURE(N == Dynamic || n == N)
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

	template <typename Real, integer N_ = 1>
	Scalar_Locator<Real, N_> scalarLocator(
		integer n = N_)
	{
		return Scalar_Locator<Real, N_>(n);
	}

}

namespace Pastel
{

	template <typename Point_>
	class Default_Locator<
		const Point_&, 
		EnableIf<std::is_arithmetic<Point_>, void>>
	{
	public:
		using Point = Point_;
		using Locator = Scalar_Locator<Point>;

		Locator operator()(const Point&) const
		{
			return Locator();
		}
	};

	template <typename Real>
	decltype(auto) scalarPoint(const Real& point)
	{
		return location(
			point,
			scalarLocator<Real>());
	}

	template <typename Real>
	decltype(auto) scalarPoint(const Real& point, integer n)
	{
		return location(
			point,
			scalarLocator<Real, Dynamic>(n));
	}

}

#endif
