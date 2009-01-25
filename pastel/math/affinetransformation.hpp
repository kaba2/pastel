#ifndef PASTELMATH_AFFINETRANSFORMATION_HPP
#define PASTELMATH_AFFINETRANSFORMATION_HPP

#include "pastel/math/affinetransformation.h"

namespace Pastel
{

	template <int N, typename Real>
	void swap(AffineTransformation<N, Real>& left,
		AffineTransformation<N, Real>& right)
	{
		left.swap(right);
	}

	template <int N, typename Real>
	Vector<N, Real> operator*(
		const Vector<N, Real>& left,
		const AffineTransformation<N, Real>& right)
	{
		return left * right.transformation();
	}

	template <int N, typename Real>
	Point<N, Real> operator*(
		const Point<N, Real>& left,
		const AffineTransformation<N, Real>& right)
	{
		return left * right.transformation() +
			right.translation();
	}

	template <int N, typename Real>
	Matrix<N + 1, N + 1, Real> asMatrix(
		const AffineTransformation<N, Real>& that)
	{
		Matrix<N + 1, N + 1, Real> result;
		const Matrix<N, N, Real>& transformation =
			that.transformation();
		const Vector<N, Real>& translation =
			that.translation();

		for (integer i = 0;i < N;++i)
		{
			result[i] = extend(transformation[i], 0);
		}
		result[N] = extend(translation, 1);

		return result;
	}

}

#endif
