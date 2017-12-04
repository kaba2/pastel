// Description: Product distance
// Documentation: distances.txt

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
	class Product_Distance
	: public DistanceBase<Product_Distance<Real, XDistance, YDistance>, Real>
	{
	public:
		using Real_ = Real;

		explicit Product_Distance(integer nx = 0)
		: nx_(nx)
		, x_()
		, y_()
		{}

		Product_Distance(const Real& y)
		: nx_(0)
		, x_()
		, y_(y)
		{}

		Product_Distance(
			integer nx,
			const XDistance& x, 
			const YDistance& y)
		: nx_(nx)
		, x_(x)
		, y_(y)
		{}

		Product_Distance(const Product_Distance&) = default;
		Product_Distance(Product_Distance&&) = default;

		explicit operator Real() const {
			using std::max;
			return max((Real)x_, (Real)y_);
		}

		Real internal() const {
			using std::max;
			return max(x_.internal(), y_.internal());
		}

		auto replace(integer axis, const Real& from, const Real& to) {
			if (axis < nx_)
				x_.replace(axis, from, to);
			else
				y_.replace(axis - nx_, from, to);
			return *this;
		}

		auto operator*=(const Real& amount) {
			x_ *= amount;
			y_ *= amount;
			return *this;
		}

	private:
		integer nx_;
		XDistance x_;
		YDistance y_;
	};

	template <
		typename Real,
		typename... Args
	>
	auto productDistance(Args&&... args)
	{
		return Product_Distance<Real, Args...>(std::forward<Args>(args)...);
	}

}

#endif
