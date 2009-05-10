/*!
\file
\brief A class for an affine transformation.
*/

#ifndef PASTELMATH_AFFINETRANSFORMATION_H
#define PASTELMATH_AFFINETRANSFORMATION_H

#include "pastel/math/matrix.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"

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

	template <int N, typename Real>
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
				PENSURE(dimension > 0);
				PENSURE(dimension == N || N == Dynamic);
			}

			//! Constructs using the given transformation and translation.
			AffineTransformation(
				const Matrix<N, N, Real>& transformation,
				const Vector<N, Real>& translation)
				: transform_(transformation)
				, translation_(translation)
			{
				BOOST_STATIC_ASSERT(N > 0);
			}

			//! Constructs using the given transformation and translation.
			AffineTransformation(
				integer dimension,
				const Matrix<N, N, Real>& transformation,
				const Vector<N, Real>& translation)
				: transform_(transformation)
				, translation_(translation)
			{
				PENSURE(dimension > 0);
				PENSURE(dimension == N || N == Dynamic);

				PENSURE(translation.dimension() == dimension);
				PENSURE(transformation.width() == dimension);
				PENSURE(transformation.height() == dimension);
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
			void set(const Matrix<N, N, Real>& transformation,
				const Vector<N, Real>& translation)
			{
				PENSURE(transform_.width() ==
					transformation.width());
				PENSURE(transform_.height() ==
					transformation.height());
				PENSURE(translation_.dimension() ==
					translation.dimension());

				transform_ = transformation;
				translation_ = translation;
			}

			integer dimension() const
			{
				return translation_.dimension();
			}

			//! Returns the transformation.
			Matrix<N, N, Real>& transformation()
			{
				return transform_;
			}

			//! Returns the transformation.
			const Matrix<N, N, Real>& transformation() const
			{
				return transform_;
			}

			//! Returns the translation.
			Vector<N, Real>& translation()
			{
				return translation_;
			}

			//! Returns the translation.
			const Vector<N, Real>& translation() const
			{
				return translation_;
			}

		private:
			Matrix<N, N, Real> transform_;
			Vector<N, Real> translation_;
	};

	typedef AffineTransformation<2, real> AffineTransformation2;
	typedef AffineTransformation<3, real> AffineTransformation3;
	typedef AffineTransformation<4, real> AffineTransformation4;

	template <int N, typename Real>
	void swap(AffineTransformation<N, Real>& left,
		AffineTransformation<N, Real>& right);

	template <int N, typename Real>
	Vector<N, Real> operator*(
		const Vector<N, Real>& left,
		const AffineTransformation<N, Real>& right);

	template <int N, typename Real>
	Point<N, Real> operator*(
		const Point<N, Real>& left,
		const AffineTransformation<N, Real>& right);

	template <int N, typename Real>
	Matrix<PASTEL_ADD_N(N, 1), PASTEL_ADD_N(N, 1), Real> asMatrix(
		const AffineTransformation<N, Real>& that);

}

#include "pastel/math/affinetransformation.hpp"

#endif
