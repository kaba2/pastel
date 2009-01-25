#ifndef PASTELMATH_POLYNOMIAL_TOOLS_H
#define PASTELMATH_POLYNOMIAL_TOOLS_H

#include "pastel/math/polynomial.h"

#include <ostream>

namespace Pastel
{

	template <typename Real>
	void differentiate(Polynomial<Real>& that);

	template <typename Real>
	Polynomial<Real> derivative(const Polynomial<Real>& that);

	template <typename Real>
	void trim(Polynomial<Real>& that);

	template <typename Real>
	void divide(
		const Polynomial<Real>& that,
		Polynomial<Real>& quontient,
		Polynomial<Real>& remainder);

	template <typename Real>
	Polynomial<Real> quotient(
		const Polynomial<Real>& that,
		const Polynomial<Real>& divider);

	template <typename Real>
	Polynomial<Real> remainder(
		const Polynomial<Real>& that,
		const Polynomial<Real>& divider);

	template <typename Real>
	void sturmSequence(
		const Polynomial<Real>& that,
		std::vector<Polynomial<Real> > sturm);

	template <typename Real>
	integer roots(
		const std::vector<Polynomial<Real> >& sturm,
		const Real& max);

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

	template <typename Real>
	std::ostream& operator<<(
		std::ostream& stream,
		const Polynomial<Real>& that);

}

#include "pastel/math/polynomial_tools.hpp"

#endif
