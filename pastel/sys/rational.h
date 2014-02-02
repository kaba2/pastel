// Description: Rational number
// Tags: C++11

#ifndef PASTELSYS_RATIONAL_H
#define PASTELSYS_RATIONAL_H

#include <pastel/sys/integer_concept.h>

#include <boost/operators.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_signed.hpp>

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
	private:
		class Private {};

		template <typename That_Integer>
		struct IsNative
			: boost::is_signed<That_Integer> 
		{};

		template <typename That_Integer>
		struct IsInteger
			: boost::mpl::and_<
			boost::mpl::not_<boost::is_signed<That_Integer>>, 
			boost::is_same<That_Integer, Integer>>
		{};

		template <typename That_Integer>
		struct IsNativeOrInteger
			: boost::mpl::or_<
			IsNative<That_Integer>,
			IsInteger<That_Integer>> 
		{};

		template <typename Left_Integer, typename Right_Integer>
		struct AreNativeOrInteger
			: boost::mpl::and_<
			IsNativeOrInteger<Left_Integer>,
			IsNativeOrInteger<Right_Integer>> 
		{};

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
		// 2.3 with Rational numbers. One has to
		// be careful to not create ambiguities 
		// when creating these overloads.
	
		//! Constructs with the value (wholes / 1).
		/*! 
		Implicit conversion allowed.
		*/
		template <typename That_Integer>
		Rational(That_Integer wholes, 
			PASTEL_ENABLE_IF(IsNativeOrInteger<That_Integer>, Private)* = 0);

		//! Constructs with the value (numerator / denominator).
		template <
			typename Numerator_Integer, 
			typename Denominator_Integer>
		Rational(
			Numerator_Integer numerator,
			Denominator_Integer denominator,
			PASTEL_ENABLE_IF((AreNativeOrInteger<Numerator_Integer, Denominator_Integer>), Private)* = 0);

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

namespace Pastel
{

	//! Computes the fractional part of x e [0, 1[.
	/*!
	This function is equivalent to remainder(x, 1).
	This version offers an optimization opportunity
	in this case.
	*/
	template <typename Integer>
	Rational<Integer> fraction(
		const Rational<Integer>& x);

	//! Computes the remainder of (x / divider) e [0, divider[.
	template <typename Integer>
	Rational<Integer> remainder(
		const Rational<Integer>& x,
		const Rational<Integer>& divider);

	// Optimization functions

	template <typename Integer>
	Rational<Integer> multiplyByPowerOf2(
		const Rational<Integer>& x,
		integer power);

	template <typename Integer>
	Rational<Integer> divideByPowerOf2(
		const Rational<Integer>& x,
		integer power);

	template <typename Integer>
	Rational<Integer> mabs(const Rational<Integer>& that);

}

#include <iostream>

namespace Pastel
{

	template <typename Integer>
	std::ostream& operator<<(std::ostream& stream,
		const Rational<Integer>& number);

}

#include "pastel/sys/rational.hpp"

#endif
