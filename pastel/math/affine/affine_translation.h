// Description: Translating affine transformation
// Documentation: affine_algorithms.txt

#ifndef PASTELMATH_AFFINE_TRANSLATION_H
#define PASTELMATH_AFFINE_TRANSLATION_H

#include "pastel/math/affine/affine_transformation.h"

namespace Pastel
{

	//! Returns a translating affine transformation.
	template <typename Real, int N>
	AffineTransformation<Real, N, N> affineTranslation(
		const Vector<Real, N>& translation)
	{
		integer n = translation.n();

		AffineTransformation<Real, N, N> result(
			identityMatrix<Real, N, N>(n, n),
			translation);

		return result;
	}

}

#endif
