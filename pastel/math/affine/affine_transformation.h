// Description: Affine transformations

#ifndef PASTELMATH_AFFINE_TRANSFORMATION_H
#define PASTELMATH_AFFINE_TRANSFORMATION_H

#include "pastel/math/matrix/matrix.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! An affine transformation in R^n.
	template <typename Real, int M, int N>
	class AffineTransformation
	{
	public:
		// Using default destructor.

		//! Constructs R^n --> R^n identity transformation.
		explicit AffineTransformation(integer n)
			: matrix_()
			, translation_(ofDimension(n))
		{
			PENSURE_OP(n, >=, 0);
			matrix_ = Matrix<Real, M, N>::Identity(n, n);
		}

		//! Move-constructs from another affine transformation.
		AffineTransformation(AffineTransformation&& that)
			: AffineTransformation(that.n())
		{
			swap(that);
		}

		//! Copy-constructs from another affine transformation.
		AffineTransformation(const AffineTransformation& that)
			: matrix_(that.matrix_)
			, translation_(that.translation_)
		{
		}

		//! Constructs using the given matrix and no translation.
		/*!
		Note: implicitly convertible.
		*/
		AffineTransformation(
			Matrix<Real, M, N> matrix)
			: matrix_(std::move(matrix))
			, translation_(ofDimension(matrix_.rows()))
		{
		}

		//! Constructs using the given matrix and translation.
		/*!
		Note: This also takes care of matrix- and vector-expressions.
		*/
		AffineTransformation(
			Matrix<Real, M, N> matrix,
			Vector<Real, N> translation)
			: matrix_(std::move(matrix))
			, translation_(std::move(translation))
		{
			PENSURE_OP(matrix_.rows(), ==, translation_.size());
		}

		//! Assigns from another affine transformation.
		AffineTransformation& operator=(AffineTransformation that)
		{
			swap(that);
			return *this;
		}

		//! Swaps two transformations.
		void swap(AffineTransformation& that)
		{
			matrix_.swap(that.matrix_);
			translation_.swap(that.translation_);
		}

		//! Assigns f := f o g.
		AffineTransformation& operator*=(
			const AffineTransformation& that)
		{
			// Let
			// f : RR^n --> RR^n : f(x) = Ax + b
			// g : RR^n --> RR^n : g(x) = Cx + d
			//
			// Then
			// (f o g)(x) = A(Cx + d) + b
			//            = ACx + (Ad + b)

			translation_ += matrix_ * that.translation_;
			matrix_ *= that.matrix_;

			return *this;
		}

		//! Returns f o g.
		AffineTransformation operator*(
			const AffineTransformation& g) const
		{
			AffineTransformation fg(*this);
			fg *= g;
			return fg;
		}

		//! Returns the dimension of the transformation.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer n() const
		{
			return translation_.n();
		}

		//! Returns the matrix.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Matrix<Real, M, N>& matrix()
		{
			return matrix_;
		}

		//! Returns the matrix.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		const Matrix<Real, M, N>& matrix() const
		{
			return matrix_;
		}

		//! Returns the translation.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Vector<Real, N>& translation()
		{
			return translation_;
		}

		//! Returns the translation.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		const Vector<Real, N>& translation() const
		{
			return translation_;
		}

	private:
		// We will not allow to default-construct
		// a 0-dimensional affine-transformation,
		// because it easily leads to bugs caused
		// by forgetting to specify the dimension.
		AffineTransformation() = delete;

		Matrix<Real, M, N> matrix_;
		Vector<Real, N> translation_;
	};

}

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

#include "pastel/math/affine/affine_inverse.h"
#include "pastel/math/affine/affine_rotation.h"
#include "pastel/math/affine/affine_scaling.h"
#include "pastel/math/affine/affine_translation.h"

#endif
