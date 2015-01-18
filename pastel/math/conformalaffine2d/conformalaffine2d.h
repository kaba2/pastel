// Description: Conformal affine tranformation in 2D

#ifndef PASTELMATH_CONFORMALAFFINE2D_H
#define PASTELMATH_CONFORMALAFFINE2D_H

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
	template <typename Real>
	class ConformalAffine2D
		: boost::multipliable<ConformalAffine2D<Real> >
	{
	public:
		// Using default copy constructor.
		// Using default destructor.

		//! Constructs the specified transformation.
		ConformalAffine2D(
			Real scaling = 1,
			Real rotation = 0);

		//! Constructs the specified transformation.
		ConformalAffine2D(
			Real scaling,
			Real rotation,
			Vector<Real> translation);

		//! Copies another transformation.
		/*!
		Exception safety: nothrow
		Time complexity: constant
		*/
		ConformalAffine2D<Real>& operator=(
			const ConformalAffine2D<Real>& that);

		//! Forms the composition that o (*this).
		/*!
		Exception safety: nothrow
		Time complexity: constant
		*/
		ConformalAffine2D<Real>& operator*=(
			const ConformalAffine2D<Real>& that);

		//! Returns the amount of scaling.
		/*!
		Exception safety: nothrow
		Time complexity: constant
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
		Time complexity: constant

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
		Time complexity: constant
		*/
		Vector<Real>& translation();

		//! Returns the translation.
		/*!
		See the documentation for the non-const version.
		*/
		const Vector<Real>& translation() const;

	private:
		Real scaling_;
		Real rotation_;
		Vector<Real> translation_;
	};

}

#include "pastel/math/conformalaffine2d/conformalaffine2d.hpp"

#include "pastel/math/conformalaffine2d/conformalaffine2d_tools.h"

#endif
