#ifndef PASTEL_AFFINETRANSFORMATION_HPP
#define PASTEL_AFFINETRANSFORMATION_HPP

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
	Vector<Real, N> operator*(
		const Vector<Real, N>& left,
		const AffineTransformation<N, Real>& right)
	{
		return left * right.transformation();
	}

	template <int N, typename Real>
	Point<Real, N> operator*(
		const Point<Real, N>& left,
		const AffineTransformation<N, Real>& right)
	{
		return left * right.transformation() +
			right.translation();
	}

	template <int N, typename Real>
	Matrix<PASTEL_ADD_N(N, 1), PASTEL_ADD_N(N, 1), Real> asMatrix(
		const AffineTransformation<N, Real>& that)
	{
		const integer dimension = that.dimension();

		Matrix<PASTEL_ADD_N(N, 1), PASTEL_ADD_N(N, 1), Real> result(
			dimension + 1, dimension + 1);
		
		const Matrix<N, N, Real>& transformation =
			that.transformation();
		const Vector<Real, N>& translation =
			that.translation();

		for (integer i = 0;i < dimension;++i)
		{
			result[i] = extend(transformation[i], 0);
		}
		result[dimension] = extend(translation, 1);

		return result;
	}

}

#endif
