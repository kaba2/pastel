// Description: Product norm
// Documentation: norms.txt

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
		typename XNorm,
		typename YNorm
	>
	class Product_Distance
	: public NormBase<Product_Distance<Real, XNorm, YNorm>, Real>
	{
	public:
		explicit Product_Distance(integer nx)
		: x_()
		, y_()
		, nx_(nx)
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
		XNorm x_;
		YNorm y_;
		integer nx_;
	};

	template <
		typename Real,
		typename... Args
	>
	auto productNorm(Args&&... args)
	{
		return Product_Distance<Real, Args...>(std::forward<Args>(args)...);
	}

}

#endif
