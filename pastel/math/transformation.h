// Description: Transformation class

#ifndef PASTEL_TRANSFORMATION_H
#define PASTEL_TRANSFORMATION_H

#include "pastel/math/affinetransformation.h"

namespace Pastel
{

	//! Transformation-inverse transformation pair.

	/*!
	The purpose of this class is to trace the transformation
	and its inverse under concatenations. This way
	one can save himself from expensive inversions.
	*/

	template <typename Real, int N>
	class Transformation
	{
	public:
		//! Construct an identity transformation.
		Transformation();

		//! Constructs a given transformation.
		// Implicit conversion allowed.
		Transformation(
			const AffineTransformation<Real, N>& that);

		//! Constructs a given transformation.
		Transformation(
			const Matrix<N, N, Real>& transformation,
			const Vector<Real, N>& translation);

		// Used for concept checking.
		~Transformation();

		//! Swaps two Transformation's.
		void swap(Transformation& that);

		//! Concatenates two transforms.
		Transformation<Real, N>& operator*=(
			const Transformation<Real, N>& that);

		//! Returns the concatenation of two transforms.
		Transformation<Real, N> operator*(
			const Transformation<Real, N>& that) const;

		//! Sets the transformation.
		void setTransform(const AffineTransformation<Real, N>& transformation);

		//! Returns the affine transformation.
		const AffineTransformation<Real, N>& affineTransform() const;

		//! Returns the affine inverse transformation.
		const AffineTransformation<Real, N>& affineInverse() const;

		//! Sets the transformation.
		void setTransform(const Matrix<N, N, Real>& transformation);

		//! Returns the transformation.
		const Matrix<N, N, Real>& transformation() const;

		//! Returns the inverse transformation.
		const Matrix<N, N, Real>& inverseTransform() const;

		//! Sets the translation
		void setTranslation(const Vector<Real, N>& translation);

		//! Returns the inverse translation.
		const Vector<Real, N>& translation() const;

		//! Returns the inverse translation.
		const Vector<Real, N>& inverseTranslation() const;

	private:
		void update() const;

		AffineTransformation<Real, N> transform_;
		mutable AffineTransformation<Real, N> inverse_;
		mutable bool update_;
	};

	typedef Transformation<real, 2> Transformation2;
	typedef Transformation<real, 3> Transformation3;
	typedef Transformation<real, 4> Transformation4;

}

#include "pastel/math/transformation.hpp"

#endif
