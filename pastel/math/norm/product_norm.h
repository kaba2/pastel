// Description: Product norm
// Documentation: norms.txt

#ifndef PASTELMATH_PRODUCT_NORM_H
#define PASTELMATH_PRODUCT_NORM_H

#include "pastel/math/norm/norm_concept.h"
#include "pastel/math/distance/product_distance.h"

namespace Pastel
{

	template <typename Real, typename XNorm, typename YNorm>
	struct Product_Norm
	{
		explicit Product_Norm(
			integer nx,
			const XNorm& xNorm,
			const YNorm& yNorm)
		: nx_(nx)
		, xNorm_(xNorm)
		, yNorm_(yNorm)
		{}

		auto operator()(const Real& distance = 0) const
		{
			auto xDistance = xNorm_(distance);
			auto yDistance = yNorm_();

			return Product_Distance<Real, decltype(xDistance), decltype(yDistance)>(nx_, xDistance, yDistance);
		}

		integer nx_;
		XNorm xNorm_;
		YNorm yNorm_;
	};

	template <
		typename Real,
		typename XNorm,
		typename YNorm
	>
	auto productNorm(const XNorm& xNorm, const YNorm& yNorm, integer nx)
	{
		return Product_Norm<Real, XNorm, YNorm>(nx);
	}

}

#endif
