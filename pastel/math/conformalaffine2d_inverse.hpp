#ifndef PASTELMATH_CONFORMALAFFINE2D_INVERSE_H
#define PASTELMATH_CONFORMALAFFINE2D_INVERSE_H

#include "pastel/math/conformalaffine2d_inverse.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real>
	ConformalAffine2D<Real> inverse(
		const ConformalAffine2D<Real>& that)
	{
		// f^-1(x) = (1 / s) Q^T x - (1 / s) Q^T t

		Real invScaling = inverse(that.scaling());

		Real c = std::cos(-that.rotation());
		Real s = std::sin(-that.rotation());
		const Real xNew = 

			-invScaling * (c * that[0] - s * that[1]); 
		Real yNew = 

			-invScaling * (s * that[0] + c * that[1]);

		Vector<Real, N> invTranslation(ofDimension(2));
		invTranslation[0] = xNew;
		invTranslation[1] = yNew;

		ConformalAffine2D<Real> result(
			invScaling,
			-that.rotation(),
			invTranslation);

		return result;
	}

}

#endif
