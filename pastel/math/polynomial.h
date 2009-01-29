#ifndef PASTELMATH_POLYNOMIAL_H
#define PASTELMATH_POLYNOMIAL_H

#include "pastel/sys/mytypes.h"

#include <boost/operators.hpp>

#include <vector>

namespace Pastel
{

	template <typename Real>
	class Polynomial
		: boost::addable<Polynomial<Real>
		, boost::subtractable<Polynomial<Real>
		, boost::multipliable<Polynomial<Real>
		, boost::addable2<Polynomial<Real>, Real
		, boost::subtractable2<Polynomial<Real>, Real
		, boost::multipliable2<Polynomial<Real>, Real
		, boost::shiftable2<Polynomial<Real>, integer
		, boost::equality_comparable<Polynomial<Real>
		> > > > > > > >
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		explicit Polynomial(integer size = 1);

		void swap(Polynomial& that);

		//! Returns the degree of the polynomial.
		/*!
		The degree of the polynomial is defined
		as the greatest index such that
		abs(that[i]) > epsilon.
		If no such index is found, the degree of the
		polynomial is 0.

		Exception safety:
		nothrow
		*/
		integer degree() const;

		void setEpsilon(const Real& epsilon);
		const Real& epsilon() const;

		void setSize(integer size);

		//! Returns the order of the contained polynomial.
		/*!
		Exception safety:
		nothrow
		*/
		integer size() const;

		//! Removes leading zeros.
		/*!
		A coefficient is deemed as zero if
		abs(coefficient) > epsilon.

		Exception safety:
		basic
		*/
		void trim();

		//! Sets a coefficient to the given value.
		/*!
		If the coefficient is beyond the current order,
		the polynomial's order is increased to cover
		the given coefficient.

		Exception safety:
		basic
		*/
		void set(integer index, const Real& that);

		//! Evaluates the polynomial at the given position.
		/*!
		Exception safety:
		nothrow, if Real addition and multiplication do not throw.
		*/
		Real operator()(const Real& x) const;

		//! Returns the index:th coefficient.
		/*!
		Preconditions:
		0 <= index < size()

		Exception safety:
		nothrow
		*/
		Real& operator[](integer index);

		//! Returns the index:th coefficient.
		/*!
		Preconditions:
		0 <= index < size()

		Exception safety:
		nothrow
		*/
		const Real& operator[](integer index) const;

		//! Adds the given value to the first coefficient.
		/*!
		Exception safety:
		nothrow, if Real += does not throw.
		*/
		Polynomial<Real>& operator+=(const Real& that);

		//! Subtracts the given value from the first coefficient.
		/*!
		Exception safety:
		nothrow, if Real -= does not throw.
		*/
		Polynomial<Real>& operator-=(const Real& that);

		//! Multiplies all coefficients with the given value.
		/*!
		Exception safety:
		nothrow, if Real *= does not throw.
		*/
		Polynomial<Real>& operator*=(const Real& that);

		//! Divides all coefficients with the given value.
		/*!
		Exception safety:
		nothrow, if Real inverse and *= do not throw.
		*/
		Polynomial<Real>& operator/=(const Real& that);

		//! Adds another polynomial to this polynomial.
		/*!
		Exception safety:
		nothrow, if Real += does not throw.
		*/
		Polynomial<Real>& operator+=(const Polynomial& that);

		//! Subtracts another polynomial from this polynomial.
		/*!
		Exception safety:
		nothrow, if Real -= does not throw.
		*/
		Polynomial<Real>& operator-=(const Polynomial& that);

		//! Multiplies another polynomial to this polynomial.
		/*!
		Exception safety:
		nothrow, if Real *, and += do not throw.
		*/
		Polynomial<Real>& operator*=(const Polynomial& that);

		//! Shifts the coefficients left by the given index.
		/*!
		Preconditions:
		index >= 0

		Exception safety:
		nothrow, if Real = does not throw.
		*/
		Polynomial<Real>& operator<<=(integer index);

		//! Shifts the coefficients right by the given index.
		/*!
		Preconditions:
		index >= 0

		Exception safety:
		nothrow, if Real = does not throw.
		*/
		Polynomial<Real>& operator>>=(integer index);

		//! Compares for equality with another polynomial.
		/*!
		Exception safety:
		nothrow, if Real == does not throw.
		*/
		bool operator==(const Polynomial& that) const;

	private:
		std::vector<Real> data_;
		Real epsilon_;
	};

}

#include "pastel/math/polynomial.hpp"

#endif
