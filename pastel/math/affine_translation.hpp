#ifndef PASTELMATH_AFFINE_TRANSLATION_HPP
#define PASTELMATH_AFFINE_TRANSLATION_HPP

#include "pastel/math/affine_translation.h"

namespace Pastel
{

	template <typename Real, int N>
	AffineTransformation<Real> affineTranslation(
		const Vector<Real, N>& translation)
	{
		integer n = translation.n();

		AffineTransformation<Real> result(
			identityMatrix<Real>(n, n),
			translation);

		return result;
	}

}

#endif
