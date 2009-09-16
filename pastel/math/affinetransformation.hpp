#ifndef PASTEL_AFFINETRANSFORMATION_HPP
#define PASTEL_AFFINETRANSFORMATION_HPP

#include "pastel/math/affinetransformation.h"

namespace Pastel
{

	template <typename Real, int N>
	void swap(AffineTransformation<Real, N>& left,
		AffineTransformation<Real, N>& right)
	{
		left.swap(right);
	}

	template <typename Real, int N>
	Matrix<Real, PASTEL_ADD_N(N, 1), PASTEL_ADD_N(N, 1)> asMatrix(
		const AffineTransformation<Real, N>& that)
	{
		const integer dimension = that.dimension();

		Matrix<Real, PASTEL_ADD_N(N, 1), PASTEL_ADD_N(N, 1)> result(
			dimension + 1, dimension + 1);
		
		const Matrix<Real, N, N>& transformation =
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

	template <typename Real, int N>
	Vector<Real, N> transformVector(
		const Vector<Real, N>& left,
		const AffineTransformation<Real, N>& right)
	{
		return left * right.transformation();
	}

	template <typename Real, int N>
	Vector<Real, N> transformPoint(
		const Vector<Real, N>& left,
		const AffineTransformation<Real, N>& right)
	{
		return left * right.transformation() + right.translation();
	}

}

#endif
