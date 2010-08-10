// Description: Conformal affine tranformation in 2D

#ifndef PASTEL_CONFORMALAFFINE2D_H
#define PASTEL_CONFORMALAFFINE2D_H

#include "pastel/sys/vector.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! A conformal affine transformation in 2D.
	/*!
	Note: While we provide the parametrization by
	the compile-time dimension N, only the values
	N == 2 and N == Dynamic are allowed. 
	*/
	template <typename Real, int N = 2>
	class ConformalAffine2D
		: boost::multipliable<ConformalAffine2D<Real, N> >
	{
	public:
		PASTEL_STATIC_ASSERT(N == 2 || N == Dynamic);

		// Using default copy constructor.
		// Using default destructor.

		//! Constructs the specified transformation.
		ConformalAffine2D(
			const Real& scaling = 1,
			const Real& rotation = 0);

		//! Constructs the specified transformation.
		ConformalAffine2D(
			const Real& scaling,
			const Real& rotation,
			const Vector<Real, N>& translation);

		//! Copies another transformation.
		/*!
		Exception safety: nothrow
		Complexity: constant
		*/
		ConformalAffine2D<Real, N>& operator=(
			const ConformalAffine2D<Real, N>& that);

		//! Forms the composition that o (*this).
		/*!
		Exception safety: nothrow
		Complexity: constant
		*/
		ConformalAffine2D<Real, N>& operator*=(
			const ConformalAffine2D<Real, N>& that);

		//! Returns the amount of scaling.
		/*!
		Exception safety: nothrow
		Complexity: constant
		*/
		Real& scaling();

		//! Returns the amount of scaling.
		/*!
		See the documentation for the non-const version.
		*/
		const Real& scaling() const;

		//! Returns the angle of rotation.
		/*!
		Exception safety: nothrow
		Complexity: constant

		The angle is measured counter-clockwise, in radians.
		*/
		Real& rotation();

		//! Returns the angle of rotation.
		/*!
		See the documentation for the non-const version.
		*/
		const Real& rotation() const;

		//! Returns the translation.
		/*!
		Exception safety: nothrow
		Complexity: constant
		*/
		Vector<Real, N>& translation();

		//! Returns the translation.
		/*!
		See the documentation for the non-const version.
		*/
		const Vector<Real, N>& translation() const;

	private:
		Real scaling_;
		Real rotation_;
		Vector<Real, N> translation_;
	};

	typedef ConformalAffine2D<real, 2> ConformalAffine2;


}

#include "pastel/math/conformalaffine2d.hpp"

#endif
