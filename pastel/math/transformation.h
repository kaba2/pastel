/*!
\file
\brief A class for a transformation-inverse pair.
*/

#ifndef PASTELMATH_TRANSFORMATION_H
#define PASTELMATH_TRANSFORMATION_H

#include "pastel/math/affinetransformation.h"

namespace Pastel
{

	//! Transformation-inverse transformation pair.

	/*!
	The purpose of this class is to trace the transformation
	and its inverse under concatenations. This way
	one can save himself from expensive inversions.
	*/

	template <int N, typename Real>
	class Transformation
	{
	public:
		//! Construct an identity transformation.
		Transformation();

		//! Constructs a given transformation.
		// Implicit conversion allowed.
		Transformation(
			const AffineTransformation<N, Real>& that);

		//! Constructs a given transformation.
		Transformation(
			const Matrix<N, N, Real>& transformation,
			const Vector<N, Real>& translation);

		// Used for concept checking.
		~Transformation();

		//! Swaps two Transformation's.
		void swap(Transformation& that);

		//! Concatenates two transforms.
		Transformation<N, Real>& operator*=(
			const Transformation<N, Real>& that);

		//! Returns the concatenation of two transforms.
		Transformation<N, Real> operator*(
			const Transformation<N, Real>& that) const;

		//! Sets the transformation.
		void setTransform(const AffineTransformation<N, Real>& transformation);

		//! Returns the affine transformation.
		const AffineTransformation<N, Real>& affineTransform() const;

		//! Returns the affine inverse transformation.
		const AffineTransformation<N, Real>& affineInverse() const;

		//! Sets the transformation.
		void setTransform(const Matrix<N, N, Real>& transformation);

		//! Returns the transformation.
		const Matrix<N, N, Real>& transformation() const;

		//! Returns the inverse transformation.
		const Matrix<N, N, Real>& inverseTransform() const;

		//! Sets the translation
		void setTranslation(const Vector<N, Real>& translation);

		//! Returns the inverse translation.
		const Vector<N, Real>& translation() const;

		//! Returns the inverse translation.
		const Vector<N, Real>& inverseTranslation() const;

	private:
		void update() const;

		AffineTransformation<N, Real> transform_;
		mutable AffineTransformation<N, Real> inverse_;
		mutable bool update_;
	};

	typedef Transformation<2, real> Transformation2;
	typedef Transformation<3, real> Transformation3;
	typedef Transformation<4, real> Transformation4;

}

#include "pastel/math/transformation.hpp"

#endif
