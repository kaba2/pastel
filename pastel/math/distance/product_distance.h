// Description: Product distance

#ifndef PASTELMATH_PRODUCT_DISTANCE_H
#define PASTELMATH_PRODUCT_DISTANCE_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/distance/distance_concept.h"
#include "pastel/math/distance/distance_base.h"

#include <algorithm>

namespace Pastel
{

	template <
		typename Real,
		typename XDistance,
		typename YDistance
	>
	class ProductDistance
	: public DistanceBase<ProductDistance<Real, XDistance, YDistance>, Real>
	{
	public:
		explicit ProductDistance(
			const XDistance& x = XDistance(),
			const YDistance& y = YDistance())
		: DistanceBase<ProductDistance<Real, XDistance, YDistance>, Real>(x.n() + y.n())
		, x_(x)
		, y_(y)
		{
		}

		ProductDistance(const ProductDistance&) = default;
		ProductDistance(ProductDistance&&) = default;

		explicit operator Real() const {
			using std::max;
			return max((Real)x_, (Real)y_);
		}

		Real internal() const {
			using std::max;
			return max(x_.internal(), y_.internal());
		}

		auto replace(integer axis, const Real& from, const Real& to) {
			if (axis < x_.n())
				x_.replace(axis, from, to);
			else
				y_.replace(axis, from, to);
			return *this;
		}

		auto operator*=(const Real& amount) {
			x_ *= amount;
			y_ *= amount;
			return *this;
		}

		auto operator/=(const Real& amount) {
			x_ *= amount;
			y_ *= amount;
			return *this;
		}

	private:
		XDistance x_;
		YDistance y_;
	};

	template <
		typename Real,
		typename... Args
	>
	auto productDistance(Args&&... args)
	{
		return ProductDistance<Real, Args...>(std::forward<Args>(args)...);
	}

}

#endif
