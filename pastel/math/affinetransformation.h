// Description: AffineTransformation class
// Detail: Allows to work with affine transformations in R^n

#ifndef PASTEL_AFFINETRANSFORMATION_H
#define PASTEL_AFFINETRANSFORMATION_H

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
			: matrix_()
			, translation_(0)
		{
			BOOST_STATIC_ASSERT(N > 0);
		}

		//! Constructs an identity transformation.
		explicit AffineTransformation(integer dimension)
			: matrix_(dimension, dimension)
			, translation_(ofDimension(dimension))
		{
			PENSURE_OP(dimension, >, 0);
			PENSURE2(dimension == N || N == Dynamic, dimension, N);
		}

		//! Constructs using the given matrix and no translation.
		explicit AffineTransformation(
			const Matrix<Real, N, N>& matrix)
			: matrix_(matrix)
			, translation_()
		{
			BOOST_STATIC_ASSERT(N > 0);
		}

		//! Constructs using the given matrix and translation.
		AffineTransformation(
			const Matrix<Real, N, N>& matrix,
			const Vector<Real, N>& translation)
			: matrix_(matrix)
			, translation_(translation)
		{
			BOOST_STATIC_ASSERT(N > 0);
		}

		//! Constructs using the given matrix and no translation.
		AffineTransformation(
			integer dimension,
			const Matrix<Real, N, N>& matrix)
			: matrix_(matrix)
			, translation_(ofDimension(dimension))
		{
			PENSURE_OP(dimension, >, 0);
			PENSURE2(dimension == N || N == Dynamic, dimension, N);

			PENSURE_OP(translation.dimension(), ==, dimension);
			PENSURE_OP(matrix.width(), ==, dimension);
			PENSURE_OP(matrix.height(), ==, dimension);
		}

		//! Constructs using the given matrix and translation.
		AffineTransformation(
			integer dimension,
			const Matrix<Real, N, N>& matrix,
			const Vector<Real, N>& translation)
			: matrix_(matrix)
			, translation_(translation)
		{
			PENSURE_OP(dimension, >, 0);
			PENSURE2(dimension == N || N == Dynamic, dimension, N);

			PENSURE_OP(translation.dimension(), ==, dimension);
			PENSURE_OP(matrix.width(), ==, dimension);
			PENSURE_OP(matrix.height(), ==, dimension);
		}

		//! Swaps two transformations.
		void swap(AffineTransformation& that)
		{
			matrix_.swap(that.matrix_);
			translation_.swap(that.translation_);
		}

		//! Combines together transformations of this and that.
		AffineTransformation& operator*=(
			const AffineTransformation& that)
		{
			matrix_ *= that.matrix_;
			translation_ = that.translation_ +
				translation_ * that.matrix_;

			return (AffineTransformation&)*this;
		}

		//! Returns the combined transformation of this and that.
		AffineTransformation operator*(
			const AffineTransformation& that) const
		{
			AffineTransformation result((AffineTransformation&)*this);
			result *= that;
			return result;
		}

		//! Sets the matrix and translation.
		void set(const Matrix<Real, N, N>& matrix,
			const Vector<Real, N>& translation)
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
		Matrix<Real, N, N>& matrix()
		{
			return matrix_;
		}

		//! Returns the matrix.
		const Matrix<Real, N, N>& matrix() const
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
		Matrix<Real, N, N> matrix_;
		Vector<Real, N> translation_;
	};

	typedef AffineTransformation<real, 2> AffineTransformation2;
	typedef AffineTransformation<real, 3> AffineTransformation3;
	typedef AffineTransformation<real, 4> AffineTransformation4;

	template <typename Real, int N>
	void swap(AffineTransformation<Real, N>& left,
		AffineTransformation<Real, N>& right);

	template <typename Real, int N>
	Matrix<Real, ModifyN<N, N + 1>::Result, ModifyN<N, N + 1>::Result> asMatrix(
		const AffineTransformation<Real, N>& that);

	template <typename Real, int N>
	Vector<Real, N> transformVector(
		const Vector<Real, N>& left,
		const AffineTransformation<Real, N>& right);

	template <typename Real, int N>
	Vector<Real, N> transformPoint(
		const Vector<Real, N>& left,
		const AffineTransformation<Real, N>& right);

}

#include "pastel/math/affinetransformation.hpp"

#endif
