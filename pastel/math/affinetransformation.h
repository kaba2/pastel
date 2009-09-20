// Description: AffineTransformation class
// Detail: Allows to work with affine transformations in R^n

#ifndef PASTEL_AFFINETRANSFORMATION_H
#define PASTEL_AFFINETRANSFORMATION_H

#include "pastel/math/matrix.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! An affine transformation in R^N.

	/*!
	An affine transformation T for points is of the form:
	x' = T(x) = xA + b
	where
	x', x are point coordinates in R^N.
	b are vector coordinates in R^N.
	A is an NxN matrix.
	An affine transformation T for vectors is of the form:
	Let v = x - y, where x and y are points and v is a vector.
	v' = T(x) - T(y) = (xA + b) - (yA + b) = xA - yA
	= (x - y)A = vA.
	An affine transformation T for normals is of the form:
	n' = n(A^-1)^T
	Proof:
	Let t be a tangent vector.
	Then t and n are orthogonal:
	tn^T = 0
	If t is transformed by a non-singular A,
	n should be transformed by a non-singular C such
	that the orthogonality is preserved. Now
	(tA)(nC)^T = 0 <=>
	tAC^Tn^T = 0

	tAC^Tn^T = tn^T
	=> AC^T = I
	=> C^T = A^-1
	=> C = (A^-1)^T = (A^T)^-1

	Let
	f(x) = xA + b
	g(x) = xC + d

	Composition:
	f(g(x)) =
	(xC + d)A + b
	= xCA + (dA + b)

	Inversion:
	Let A and C be invertible.
	for all x: f(g(x)) = xCA + (dA + b) = x
	<=>
	CA = I and dA + b = 0
	<=>
	C = A^-1
	d = -bA^-1
	<=>
	g(x) = f^-1(x) = (x - b)A^-1
	*/

	template <typename Real, int N>
	class AffineTransformation
	{
	public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			//! Constructs an identity transformation.
			AffineTransformation()
				: transform_()
				, translation_(0)
			{
				BOOST_STATIC_ASSERT(N > 0);
			}

			//! Constructs an identity transformation.
			explicit AffineTransformation(integer dimension)
				: transform_(dimension, dimension)
				, translation_(ofDimension(dimension), 0)
			{
				PENSURE_OP(dimension, >, 0);
				PENSURE2(dimension == N || N == Dynamic, dimension, N);
			}

			//! Constructs using the given transformation and translation.
			AffineTransformation(
				const Matrix<Real, N, N>& transformation,
				const Vector<Real, N>& translation)
				: transform_(transformation)
				, translation_(translation)
			{
				BOOST_STATIC_ASSERT(N > 0);
			}

			//! Constructs using the given transformation and translation.
			AffineTransformation(
				integer dimension,
				const Matrix<Real, N, N>& transformation,
				const Vector<Real, N>& translation)
				: transform_(transformation)
				, translation_(translation)
			{
				PENSURE_OP(dimension, >, 0);
				PENSURE2(dimension == N || N == Dynamic, dimension, N);

				PENSURE_OP(translation.dimension(), ==, dimension);
				PENSURE_OP(transformation.width(), ==, dimension);
				PENSURE_OP(transformation.height(), ==, dimension);
			}

			void swap(AffineTransformation& that)
			{
				transform_.swap(that.transform_);
				translation_.swap(that.translation_);
			}

			//! Combines together transformations of this and that.
			AffineTransformation& operator*=(
				const AffineTransformation& that)
			{
				transform_ *= that.transform_;
				translation_ = that.translation_ +
					translation_ * that.transform_;

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

			//! Sets the transformation and translation.
			void set(const Matrix<Real, N, N>& transformation,
				const Vector<Real, N>& translation)
			{
				PENSURE_OP(transform_.width(), ==, transformation.width());
				PENSURE_OP(transform_.height(), ==, transformation.height());
				PENSURE_OP(translation_.dimension(), ==, translation.dimension());

				transform_ = transformation;
				translation_ = translation;
			}

			integer dimension() const
			{
				return translation_.dimension();
			}

			//! Returns the transformation.
			Matrix<Real, N, N>& transformation()
			{
				return transform_;
			}

			//! Returns the transformation.
			const Matrix<Real, N, N>& transformation() const
			{
				return transform_;
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
			Matrix<Real, N, N> transform_;
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
