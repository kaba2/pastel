// Description: Algorithms for polynomials
// Detail: differentiate, derivative, trim, divide, randomPolynomial, etc.

#ifndef PASTELMATH_POLYNOMIAL_TOOLS_H
#define PASTELMATH_POLYNOMIAL_TOOLS_H

#include "pastel/math/polynomial/polynomial.h"

#include <ostream>

namespace Pastel
{

	//! Differentiates a polynomial.
	template <typename Real>
	void differentiate(Polynomial<Real>& that);

	//! Returns the derivative of a polynomial.
	template <typename Real>
	Polynomial<Real> derivative(const Polynomial<Real>& that);

	//! Removes leading zero factors.
	template <typename Real>
	void trim(Polynomial<Real>& that);

	//! Divides a polynomial by another.
	template <typename Real>
	void divide(
		const Polynomial<Real>& that,
		const Polynomial<Real>& divider,
		Polynomial<Real>& quotient,
		Polynomial<Real>& remainder);

	//! Returns the quotient of dividing a polynomial by another.
	template <typename Real>
	Polynomial<Real> quotient(
		const Polynomial<Real>& that,
		const Polynomial<Real>& divider);

	//! Returns the remainder of dividing a polynomial by another.
	template <typename Real>
	Polynomial<Real> remainder(
		const Polynomial<Real>& that,
		const Polynomial<Real>& divider);

	//! Computes the Sturm-sequence of a polynomial.
	template <typename Real>
	void sturmSequence(
		const Polynomial<Real>& that,
		std::vector<Polynomial<Real> > sturm);

	//! Returns the number of roots before 'max'.
	/*!
	'sturm' must be a Sturm-sequence.
	*/
	template <typename Real>
	integer roots(
		const std::vector<Polynomial<Real> >& sturm,
		const Real& max);

	//! Returns the number of roots in the interval [min, max].
	/*!
	'sturm' must be a Sturm-sequence.
	*/
	template <typename Real>
	integer roots(
		const std::vector<Polynomial<Real> >& sturm,
		const Real& min, const Real& max);

	//! Returns a random polynomial of the given degree.
	/*!
	The coefficients of the polynomial will
	be chosen uniformly from the range [0, 1].
	*/
	template <typename Real>
	Polynomial<Real> randomPolynomial(
		integer degree);

	//! Pretty-prints a polynomial to a stream.
	template <typename Real>
	std::ostream& operator<<(
		std::ostream& stream,
		const Polynomial<Real>& that);

}

#include "pastel/math/polynomial/polynomial_tools.hpp"

#endif
