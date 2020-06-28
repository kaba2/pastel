#ifndef PASTELMATH_AFFINE_TRANSFORMATION_HPP
#define PASTELMATH_AFFINE_TRANSFORMATION_HPP

#include "pastel/math/affine/affine_transformation.h"

namespace Pastel
{

	//! Swaps two affine transformations.
	template <typename Real, int M, int N>
	void swap(
		AffineTransformation<Real, M, N>& left,
		AffineTransformation<Real, M, N>& right)
	{
		left.swap(right);
	}

	//! Returns the affine transformation as a homogeneous matrix.
	template <typename Real, int M, int N>
	Matrix<Real, AddN<M>, AddN<N>> asMatrix(
		const AffineTransformation<Real, M, N>& that)
	{
		integer m = that.rows();
		integer n = that.cols();
		Matrix<Real, AddN<M>, AddN<N>> result = Matrix<Real, AddN<M>, AddN<N>>::Zero(m + 1, n + 1);
		for (integer i = 0;i < n;++i)
		{
			result.col(i) = extend(that.matrix().col(i), 0);
		}
		result.col(n) = extend(that.translation(), 1);

		return result;
	}

	//! Transforms a vector by the affine transformation.
	template <typename Real, int M, int N>
	Vector<Real, N> transformVector(
		const AffineTransformation<Real, M, N>& affine,
		const Vector<Real, N>& vector)
	{
		// Ax
		return affine.matrix() * vector;
	}

	//! Transforms a point by the affine transformation.
	template <typename Real, int M, int N, int NV>
	Vector<Real, N> transformPoint(
		const AffineTransformation<Real, M, N>& affine,
		const Vector<Real, NV>& point)
	{
		// Ax + b
		return affine.matrix() * point + affine.translation();
	}

	//! Transforms a normal by the affine transformation.
	template <typename Real, int M, int N, int NV>
	Vector<Real, N> transformNormal(
		const AffineTransformation<Real, M, N>& affineInverse,
		const Vector<Real, NV>& normal)
	{
		// A^(-T) x
		return transpose(affineInverse.matrix()) * normal;
	}

}

#endif
