// Description: Rational class
// Detail: Allows to work with rational numbers
// Tags: C++11

#ifndef PASTEL_RATIONAL_H
#define PASTEL_RATIONAL_H

#include <boost/operators.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace Pastel
{

	//! A rational number.
	template <typename Integer>
	class Rational
		: boost::totally_ordered<
		Rational<Integer>
		, boost::field_operators1<
		Rational<Integer>
		> >
	{
	public:
		// Using default destructor.

		//! Constructs with the value (0 / 1).
		Rational();

		//! Copy-constructs with another number.
		Rational(const Rational& that);

		//! Move-constructs with another number.
		Rational(Rational&& that);

		// Note in the following the different
		// kinds of constructors. The intent is
		// to identify literals such as 1, 2.3f,
		// 2.3 with Rational numbers. This is
		// why we provide
		// Rational(integer wholes);
		// Rational(real32_ieee that);
		// Rational(real64_ieee that);

		//! Constructs with the value (wholes / 1).
		/*! 
		Implicit conversion allowed.
		*/
		Rational(integer wholes);

		//! Constructs with the value (wholes / 1).
		/*! 
		Implicit conversion allowed.
		This constructor causes an amgiguity
		if integer == Integer. Thus we "remove"
		this constructor in that case by changing
		it to take an EmptyClass which we mean
		to never be used.
		*/
		Rational(typename boost::mpl::if_<std::is_same<integer, Integer>, EmptyClass, Integer>::type wholes);

		//! Constructs with the value (numerator / denominator).
		Rational(Integer numerator,
				 Integer denominator);

		//! Constructs with the value of the ieee single floating point.
		/*!
		Implicit conversion allowed.
		*/
		Rational(real32_ieee that);

		//! Constructs with the value of the ieee double floating point.
		/*! 
		Implicit conversion allowed.
		*/
		Rational(real64_ieee that);

		//! Assigns another rational number.
		Rational<Integer>& operator=(Rational that);

		//! Swaps two rational numbers.
		void swap(Rational& that);

		//! Sets the value to (numerator / denominator).
		void set(
			Integer numerator,
			Integer denominator);

		//! Returns the numerator.
		const Integer& numerator() const;

		//! Returns the denominator.
		const Integer& denominator() const;

		//! Adds the given number to this number.
		Rational<Integer>& operator+=(Rational that);

		//! Subtracts the given number from this number.
		Rational<Integer>& operator-=(Rational that);

		//! Multiplies this number with the given number.
		Rational<Integer>& operator*=(Rational that);

		//! Divides this number with the given number.
		Rational<Integer>& operator/=(Rational that);

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
		friend bool operator==(
			const Rational& left,
			const Rational& right)
		{
			return left.equal(right);
		}

		//! Returns if this number is less than the given number.
		friend bool operator<(
			const Rational& left,
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
		
		class SkipSimplify {};

		//! Constructs with the value (numerator / denominator).
		Rational(Integer numerator,
				 Integer denominator,
				 SkipSimplify);

		//! Sets the value to (numerator / denominator).
		void set(
			Integer numerator,
			Integer denominator,
			SkipSimplify);

		//! Brings the rational number to a normal form.
		/*!
		If the number is not NaN (0, 0), then the normal
		form is where gcd(numerator, denominator) = 1
		and numerator >= 0.	The normal form for NaN is 
		NaN itself.
		*/
		void simplify();

		typename NumberType::Enum classify() const;

		bool lessThan(const Rational& that) const;
		bool equal(const Rational& that) const;

		Integer numerator_;
		Integer denominator_;
	};

}

#include "pastel/sys/rational.hpp"

#include "pastel/sys/rational_more.h"

#endif
