// Description: Product norm
// Documentation: norms.txt

#ifndef PASTELMATH_PRODUCT_NORM_H
#define PASTELMATH_PRODUCT_NORM_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/norm/norm_concept.h"
#include "pastel/math/norm/norm_base.h"

#include <algorithm>

namespace Pastel
{

	template <
		typename Real,
		typename XNorm,
		typename YNorm
	>
	class Product_Norm
	: public NormBase<Product_Norm<Real, XNorm, YNorm>, Real>
	{
	public:
		explicit Product_Norm(integer nx)
		: x_()
		, y_()
		, nx_(nx)
		{}

		Product_Norm(const Product_Norm&) = default;
		Product_Norm(Product_Norm&&) = default;

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
		return Product_Norm<Real, Args...>(std::forward<Args>(args)...);
	}

}

#endif
