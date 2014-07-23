#ifndef PASTELMATH_AFFINE_TRANSFORMATION_HPP
#define PASTELMATH_AFFINE_TRANSFORMATION_HPP

#include "pastel/math/affine_transformation.h"

namespace Pastel
{

	template <typename Real>
	void swap(
		AffineTransformation<Real>& left,
		AffineTransformation<Real>& right)
	{
		left.swap(right);
	}

	template <typename Real>
	Matrix<Real> asMatrix(
		const AffineTransformation<Real>& that)
	{
		integer n = that.n();
		Matrix<Real> result(n + 1, n + 1);
		for (integer i = 0;i < n;++i)
		{
			result.column(i) = extend(that.matrix().column(i), 0);
		}
		result.column(n) = extend(that.translation(), 1);

		return result;
	}

	template <typename Real, int N>
	Vector<Real, N> transformVector(

		const AffineTransformation<Real>& affine,
		const Vector<Real, N>& vector)
	{
		// Ax
		return affine.matrix() * vector;
	}

	template <typename Real, int N>
	Vector<Real, N> transformPoint(
		const AffineTransformation<Real>& affine,
		const Vector<Real, N>& point)
	{
		// Ax + b
		return affine.matrix() * point + affine.translation();
	}

	template <typename Real, int N>
	Vector<Real, N> transformNormal(
		const AffineTransformation<Real>& affineInverse,
		const Vector<Real, N>& normal)
	{
		// A^(-T) x
		return transpose(affineInverse.matrix()) * normal;
	}

}

#endif
