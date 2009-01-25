/*!
\file
\brief A class for a rational number.
*/

#ifndef PASTELMATH_RATIONAL_H
#define PASTELMATH_RATIONAL_H

#include <boost/operators.hpp>

namespace Pastel
{

	//! A rational number.

	/*!
	This class can be used in conjuction with integer
	types to provide a rational number. Rationals
	have the potential to provide exact arithmetic
	under restricted situations.

	There are several values with a special meaning:
	(0/0) is called 'Not-A-Number' (NaN).
	(x/0) for any x > 0 is called 'Infinity(x)'.
	(y/0) for any y < 0 is called 'MinusInfinity(x)'.
	It holds that:
	Infinity(x) = Infinity(y), for any x,y != 0.
	MinusInfinity(x) = -Infinity(y), for any x,y != 0.
	Also, in calculations Infinity(x) delivers the same
	results as using Infinity(y), for any x, y != 0.
	Thus we can simply speak of 'Infinity' and '-Infinity'.

	This rational class maintains two invariants:
	* If the number is not NaN, GCD(numerator, denominator) = 1.
	If the number is NaN, GCD is undefined.
	* denominator >= 0.
	*/

	template <typename Integer>
	class Rational
		: boost::less_than_comparable<
		Rational<Integer>
		, boost::equality_comparable<
		Rational<Integer>
		, boost::field_operators1<
		Rational<Integer>
		> > >
	{
	public:
		// Using default copy constructor.
		// Using default destructor.

		//! Constructs with the value (0 / 1).
		Rational();

		//! Constructs with the value (wholes / 1).
		// Implicit conversion allowed.
		Rational(integer wholes);

		//! Constructs with the value (wholes / 1).
		// Implicit conversion allowed.
		Rational(const Integer& wholes);

		//! Constructs with the value (numerator / denominator).
		Rational(const Integer& numerator,
				 const Integer& denominator);

		//! Constructs with the value of the ieee floating point.
		Rational(real32_ieee that);

		//! Assigns another rational number.
		Rational<Integer>& operator=(const Rational<Integer>& that);

		//! Swaps two rational numbers.
		void swap(Rational<Integer>& that);

		//! Sets the value to (numerator / denominator).
		void set(const Integer& numerator,
			const Integer& denominator);

		//! Returns the numerator.
		const Integer& numerator() const;

		//! Returns the denominator.
		const Integer& denominator() const;

		//! Adds the given number to this number.
		Rational<Integer>& operator+=(const Rational<Integer>& that);

		//! Subtracts the given number from this number.
		Rational<Integer>& operator-=(const Rational<Integer>& that);

		//! Multiplies this number with the given number.
		Rational<Integer>& operator*=(const Rational<Integer>& that);

		//! Divides this number with the given number.
		Rational<Integer>& operator/=(const Rational<Integer>& that);

		//! Returns the negation of this number.
		Rational<Integer> operator-() const;

		//! Returns this number.
		Rational<Integer> operator+() const;

		// The following operators must be defined as global friends.
		// Note for boost::operators to work the operators must
		// be in this class scope, thus you can't turn them
		// into template functions.
		// If you make these operators member functions,
		// the combination of implicit conversions from integer
		// and Integer and the boost::operators does not work.
		// I suspect it is because the member function
		// operator== and operator< hide the same-named
		// functions in the base class.

		//! Returns if the given number equals this number.
		friend bool operator==(const Rational& left,
			const Rational& right)
		{
			return left.equal(right);
		}

		//! Returns if this number is less than the given number.
		friend bool operator<(const Rational& left,
			const Rational& right)
		{
			return left.lessThan(right);
		}

	private:
		class NumberType
		{
		public:
			enum Enum
			{
				Normal,
				Nan,
				Infinity,
				MinusInfinity,
				Zero
			};
		};

		void simplify();
		typename NumberType::Enum classify() const;

		bool lessThan(const Rational& that) const;
		bool equal(const Rational& that) const;

		Integer numerator_;
		Integer denominator_;
	};

}

#include "pastel/math/rational.hpp"

#include "pastel/math/rational_more.h"

#endif
