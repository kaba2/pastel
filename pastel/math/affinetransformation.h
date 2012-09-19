// Description: AffineTransformation class
// Detail: Allows to work with affine transformations in R^n

#ifndef PASTELMATH_AFFINETRANSFORMATION_H
#define PASTELMATH_AFFINETRANSFORMATION_H

#include "pastel/math/matrix.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! An affine transformation in R^N.

	template <typename Real, int N>
	class AffineTransformation
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		//! Constructs an identity transformation.
		AffineTransformation()
			: matrix_(identityMatrix<Real>(N, N))
			, translation_(0)
		{
			PASTEL_STATIC_ASSERT(N > 0);
		}

		//! Constructs an identity transformation.
		explicit AffineTransformation(integer n)
			: matrix_(n, n)
			, translation_(ofDimension(n))
		{
			PENSURE_OP(n, >, 0);
			PENSURE2(n == N || N == Dynamic, n, N);
		}

		//! Constructs using the given matrix and no translation.
		template <typename Expression>
		explicit AffineTransformation(
			const MatrixExpression<Real, Expression>& matrix)
			: matrix_(matrix)
			, translation_(ofDimension(matrix_.m()))
		{
		}

		//! Constructs using the given matrix and translation.
		template <typename Left_Expression, typename Right_Expression>
		AffineTransformation(
			const MatrixExpression<Real, Left_Expression>& matrix,
			const VectorExpression<Real, N, Right_Expression>& translation)
			: matrix_(matrix)
			, translation_(translation)
		{
			PASTEL_STATIC_ASSERT(N > 0);
			PENSURE_OP(matrix_.m(), ==, translation_.size());
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

		//! Returns the combined transformation of this and that.
		AffineTransformation operator*(
			const AffineTransformation& that) const
		{
			AffineTransformation result(*this);
			result *= that;
			return result;
		}

		//! Sets the matrix and translation.
		template <typename Left_Expression, typename Right_Expression>
		void set(
			const MatrixExpression<Real, Left_Expression>& matrix,
			const VectorExpression<Real, N, Right_Expression>& translation)
		{
			PENSURE_OP(matrix_.width(), ==, matrix.width());
			PENSURE_OP(matrix_.height(), ==, matrix.height());
			PENSURE_OP(translation_.dimension(), ==, translation.dimension());

			matrix_ = matrix;
			translation_ = translation;
		}

		//! Returns the dimension of the transformation.
		integer dimension() const
		{
			return translation_.dimension();
		}

		//! Returns the matrix.
		Matrix<Real>& matrix()
		{
			return matrix_;
		}

		//! Returns the matrix.
		const Matrix<Real>& matrix() const
		{
			return matrix_;
		}

		//! Returns the translation.
		Vector<Real, N>& translation()
		{
			return translation_;
		}

		//! Returns the translation.
		const Vector<Real, N>& translation() const
		{
			return translation_;
		}

	private:
		Matrix<Real> matrix_;
		Vector<Real, N> translation_;
	};

	typedef AffineTransformation<real, 2> AffineTransformation2;
	typedef AffineTransformation<real, 3> AffineTransformation3;
	typedef AffineTransformation<real, 4> AffineTransformation4;

	template <typename Real, int N>
	void swap(AffineTransformation<Real, N>& left,
		AffineTransformation<Real, N>& right);

	template <typename Real, int N>
	Matrix<Real> asMatrix(
		const AffineTransformation<Real, N>& that);

	template <typename Real, int N>
	Vector<Real, N> transformVector(
		const Vector<Real, N>& left,
		const AffineTransformation<Real, N>& right);

	template <typename Real, int N>
	Vector<Real, N> transformPoint(
		const Vector<Real, N>& left,
		const AffineTransformation<Real, N>& right);

	template <typename Real, int N>
	Vector<Real, N> transformNormal(
		const Vector<Real, N>& left,
		const AffineTransformation<Real, N>& rightInverse);

}

#include "pastel/math/affinetransformation.hpp"

#include "pastel/math/affinetransformation_tools.h"

#endif
