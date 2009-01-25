/*!
\file
\brief Functions for computing with complex numbers.
*/

#ifndef PASTELMATH_COMPLEX_TOOLS_H
#define PASTELMATH_COMPLEX_TOOLS_H

#include "pastel/math/complex.h"

namespace Pastel
{

	//! Returns the inverse of the given number.

	template <typename Real>
	Complex<Real> inverse(const Complex<Real>& x);

	//! Returns the conjugate of the given number.

	template <typename Real>
	Complex<Real> conjugate(const Complex<Real>& x);

	//! Returns the modulus of the given number.

	template <typename Real>
	Real mod(const Complex<Real>& x);

	//! Returns the argument of the given number.

	template <typename Real>
	Real arg(const Complex<Real>& x);

	//! Returns the sine of the given number.

	template <typename Real>
	Complex<Real> sin(
		const Complex<Real>& x);

	//! Returns the cosine of the given number.

	template <typename Real>
	Complex<Real> cos(
		const Complex<Real>& x);

	//! Returns the tangent of the given number.

	template <typename Real>
	Complex<Real> tan(
		const Complex<Real>& x);

	//! Returns the cotangent of the given number.

	template <typename Real>
	Complex<Real> cot(
		const Complex<Real>& x);

	//! Returns the natural exponential of the given number.

	template <typename Real>
	Complex<Real> exp(
		const Complex<Real>& x);

	//! Returns the 'base' raised to the power of 'exponent'.

	template <typename Real>
	Complex<Real> pow(
		const Complex<Real>& base,
		const Complex<Real>& exponent);

	//! Returns the logarithm of the given number.

	template <typename Real>
	Complex<Real> log(const Complex<Real>& x);

	//! Returns the complex number corresponding to a polar description.

	template <typename Real>
	Complex<Real> polar(const Real& mod, const Real& arg);

	//! Returns the square root of the given number.

	template <typename Real>
	Complex<Real> sqrt(const Real& x);

}

#include "pastel/math/complex_tools.hpp"

#endif
