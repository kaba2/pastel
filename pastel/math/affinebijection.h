// Description: AffineBijection class
// Detail: Traces an affine transformation and its inverse.

#ifndef PASTELMATH_AFFINEBIJECTION_H
#define PASTELMATH_AFFINEBIJECTION_H

#include "pastel/math/affinetransformation.h"

namespace Pastel
{

	//! Affine transformation pair.
	/*!
	The purpose of this class is to trace the transformation
	and its inverse under concatenations. This way
	one can save himself from expensive inversions, since
	the inverse is always cached.
	*/

	template <typename Real, int N>
	class AffineBijection
	{
	public:
		//! Construct an identity transformation.
		AffineBijection();

		//! Constructs a given transformation.
		// Implicit conversion allowed.
		AffineBijection(
			const AffineTransformation<Real, N>& that);

		//! Constructs a given transformation.
		AffineBijection(
			const Matrix<Real, N, N>& transformation,
			const Vector<Real, N>& translation);

		// Used for concept checking.
		~AffineBijection();

		//! Swaps two AffineBijection's.
		void swap(AffineBijection& that);

		//! Concatenates two transforms.
		AffineBijection<Real, N>& operator*=(
			const AffineBijection<Real, N>& that);

		//! Returns the concatenation of two transforms.
		AffineBijection<Real, N> operator*(
			const AffineBijection<Real, N>& that) const;

		//! Sets the forward transformation.
		void setTransform(const AffineTransformation<Real, N>& transformation);

		//! Returns the forward transformation.
		const AffineTransformation<Real, N>& forward() const;

		//! Returns the inverse transformation.
		const AffineTransformation<Real, N>& inverse() const;

		//! Sets the forward transformation.
		void setMatrix(const Matrix<Real, N, N>& transformation);

		//! Returns the forward transformation.
		const Matrix<Real, N, N>& matrix() const;

		//! Returns the inverse transformation.
		const Matrix<Real, N, N>& inverseMatrix() const;

		//! Sets the forward translation
		void setTranslation(const Vector<Real, N>& translation);

		//! Returns the forward translation.
		const Vector<Real, N>& translation() const;

		//! Returns the inverse translation.
		const Vector<Real, N>& inverseTranslation() const;

	private:
		void update() const;

		AffineTransformation<Real, N> transform_;
		mutable AffineTransformation<Real, N> inverse_;
		mutable bool update_;
	};

	typedef AffineBijection<real, 2> AffineBijection2;
	typedef AffineBijection<real, 3> AffineBijection3;
	typedef AffineBijection<real, 4> AffineBijection4;

}

#include "pastel/math/affinebijection.hpp"

#endif
