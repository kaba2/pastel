/*!
\file
\brief A class for a complex number.
*/

#ifndef PASTELMATH_COMPLEX_H
#define PASTELMATH_COMPLEX_H

namespace Pastel
{

	//! A complex number.

	/*!
	The interface of this class is
	functionally identical
	to that of the STL's 'complex' class. This class
	can be used as a direct drop-in for it.
	One substantial difference is that the real
	and imaginary parts can be accessed as
	mutable references. This is not so in STL, which
	took the route of abstracting the storage
	of the complex number. This class works
	with any type that models the Complex concept.
	*/

	template <typename Real>
	class Complex
	{
	public:

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		typedef Real value_type;
		typedef Real ValueType;

		//! Constructs to (0, 0)
		Complex();

		//! Constructs to (re, 0)
		Complex(const Real& re);

		//! Constructs to (re, im)
		Complex(const Real& re, const Real& im);

		//! Constructs using a different type of complex number.

		// Note that the copy constructor won't match
		// this function.

		template <typename Other>
		Complex(const Complex<Other>& that);

		//! Assigns from a different type of complex number.

		// Note that self-assignment won't match
		// this function.

		template <typename Other>
		Complex<Real>& operator=(const Complex<Other>& that);

		//! Adds from any type of complex number.

		template <typename Other>
		Complex<Real>& operator+=(const Complex<Other>& that);

		//! Subtracts from any type of complex number.

		template <typename Other>
		Complex<Real>& operator-=(const Complex<Other>& that);

		//! Multiplies from any type of complex number.

		template <typename Other>
		Complex<Real>& operator*=(const Complex<Other>& that);

		//! Divides from any type of complex number.

		template <typename Other>
		Complex<Real>& operator/=(const Complex<Other>& that);

		//! Adds a real number.

		Complex<Real>& operator+=(const Real& that);

		//! Subtracts a real number.

		Complex<Real>& operator-=(const Real& that);

		//! Multiplies by a real number.

		Complex<Real>& operator*=(const Real& that);

		//! Divides by a real number.

		Complex<Real>& operator/=(const Real& that);

		//! Returns the result of adding together this and that.

		template <typename Other>
		Complex<Real> operator+(const Complex<Other>& that) const;

		//! Returns the result of subtracting that from this.

		template <typename Other>
		Complex<Real> operator-(const Complex<Other>& that) const;

		//! Returns the result of multiplying together this and that.

		template <typename Other>
		Complex<Real> operator*(const Complex<Other>& that) const;

		//! Returns the result of dividing this by that.

		template <typename Other>
		Complex<Real> operator/(const Complex<Other>& that) const;

		//! Returns the result of summing together this and real that.

		Complex<Real> operator+(const Real& that) const;

		//! Returns the result of subtracting a real that from this.

		Complex<Real> operator-(const Real& that) const;

		//! Returns the result of multiplying together this and real that.

		Complex<Real> operator*(const Real& that) const;

		//! Returns the result of dividing this by that.

		Complex<Real> operator/(const Real& that) const;

		//! Returns the negation of the number.

		Complex<Real> operator-() const;

		//! Returns the real part of the number.

		Real& real();

		//! Returns the real part of the number.

		const Real& real() const;

		//! Returns the imaginary part of the number.

		Real& imag();

		//! Returns the imaginary part of the number.

		const Real& imag() const;

	private:
		Real re_;
		Real im_;
	};

}

#include "pastel/math/complex.hpp"

#endif
