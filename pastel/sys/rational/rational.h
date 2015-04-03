// Description: Rational number
// Tags: C++11

#ifndef PASTELSYS_RATIONAL_H
#define PASTELSYS_RATIONAL_H

#include <pastel/sys/integer/integer_concept.h>

#include <boost/operators.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>

namespace Pastel
{

	namespace Rational_
	{

		template <typename That_Integer>
		struct IsNative
			: std::is_integral<That_Integer> 
		{};

		template <
			typename Integer,
			typename That_Integer>
		struct IsInteger
			: boost::mpl::and_<
			boost::mpl::not_<std::is_integral<That_Integer>>, 
			std::is_same<That_Integer, Integer>>
		{};

		template <
			typename Integer,
			typename That_Integer>
		struct IsNativeOrInteger
			: boost::mpl::or_<
			IsNative<That_Integer>,
			IsInteger<Integer, That_Integer>> 
		{};

		template <
			typename Integer, 
			typename Left_Integer, 
			typename Right_Integer>
		struct AreNativeOrInteger
			: boost::mpl::and_<
			IsNativeOrInteger<Integer, Left_Integer>,
			IsNativeOrInteger<Integer, Right_Integer>> 
		{};

	}

	//! A rational number.
	template <typename Integer_>
	class Rational
		: boost::totally_ordered<Rational<Integer_>
		, boost::field_operators1<Rational<Integer_>
		, boost::unit_steppable<Rational<Integer_>
		> > >
	{
	public:
		using Integer = Integer_;

		//PASTEL_CONCEPT_CHECK(Integer, Integer_Concept);

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
		template <
			typename That_Integer,
			EnableIf<Rational_::IsNativeOrInteger<Integer, That_Integer>> = 0>
		Rational(That_Integer wholes);

		//! Constructs with the value (m / n).
		template <
			typename M_Integer, 
			typename N_Integer,
			EnableIf<Rational_::AreNativeOrInteger<Integer, M_Integer, N_Integer>> = 0>
		Rational(
			M_Integer m,
			N_Integer n);

		//! Constructs with a native floating point number.
		/*!
		Implicit conversion allowed.

		Preconditions:
		nMax >= 0

		nMax:
		The maximum allowed divisor. Setting this to 0
		removes any restrictions.

		returns:
		On normal numbers, the best rational approximation.
		On positive overflow, infinity<Rational>().
		On negative overflow, -infinity<Rational>().
		On underflow, 0.
		*/
		template <
			typename Real,
			EnableIf<std::is_floating_point<Real>> = 0>
		Rational(
			Real that,
			Integer nMax = 0);

		//! Assigns another rational number.
		Rational<Integer>& operator=(Rational that);

		//! Swaps two rational numbers.
		void swap(Rational& that);

		//! Sets the value to (m / n).
		void set(
			Integer m,
			Integer n);

		//! Returns the m.
		const Integer& m() const;

		//! Returns the n.
		const Integer& n() const;

		//! Returns an approximating real number.
		template <
			typename Real,
			EnableIf<std::is_floating_point<Real>> = 0>
		Real asReal() const;

		//! Returns the number as a ratio of integers.
		std::string asStringRatio() const;

		//! Returns a b-base expansion of the number.
		/*!
		Preconditions:
		base >= 2
		maxDigits >= 0

		base:
		The base to use.

		showBase:
		Whether to show the base.

		maxDigits:
		The number of digits to show for the 
		fractional part.
		*/
		std::string asString(
			integer base = 10, 
			integer maxDigits = 3,
			bool showBase = false) const;

		//! Returns whether the number is infinity.
		bool isInfinity() const;

		//! Returns whether the number is not-a-number.
		bool isNan() const;

		//! Returns whether the number is an integer.
		bool isInteger() const;

		//! Returns whether the number is == 0.
		bool isZero() const
		{
			return zero(*this);
		}

		//! Returns whether the number is > 0.
		bool isPositive() const
		{
			return positive(*this);
		}

		//! Returns whether the number is < 0.
		bool isNegative() const
		{
			return negative(*this);
		}

		//! Adds the given number to this number.
		Rational<Integer>& operator+=(Rational that);

		//! Subtracts the given number from this number.
		Rational<Integer>& operator-=(Rational that);

		//! Multiplies this number with the given number.
		Rational<Integer>& operator*=(Rational that);

		//! Divides this number with the given number.
		Rational<Integer>& operator/=(Rational that);

		//! Adds 1 to this number.
		Rational<Integer>& operator++()
		{
			*this += 1;
			return *this;
		}

		//! Subtracts 1 from this number.
		Rational<Integer>& operator--()
		{
			*this -= 1;
			return *this;
		}

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
		enum class NumberType : integer
		{
			Normal,
			Nan,
			Infinity,
			MinusInfinity,
			Zero
		};
		
		class SkipSimplify {};

		//! Constructs with the value (m / n).
		Rational(Integer m,
				 Integer n,
				 SkipSimplify);

		//! Sets the value to (m / n).
		void set(
			Integer m,
			Integer n,
			SkipSimplify);

		//! Brings the rational number to a normal form.
		/*!
		If the number is not NaN (0, 0), then the normal
		form is where gcd(m, n) = 1
		and m >= 0.	The normal form for NaN is 
		NaN itself.
		*/
		void simplify();

		NumberType classify() const;

		bool lessThan(const Rational& that) const;
		bool equal(const Rational& that) const;

		Integer m_;
		Integer n_;
	};

}

#include "pastel/sys/rational/rational.hpp"
#include "pastel/sys/rational/rational_add.hpp"
#include "pastel/sys/rational/rational_as_point.hpp"
#include "pastel/sys/rational/rational_as_real.hpp"
#include "pastel/sys/rational/rational_classify.hpp"
#include "pastel/sys/rational/rational_compare.hpp"
#include "pastel/sys/rational/rational_construct.hpp"
#include "pastel/sys/rational/rational_multiply.hpp"
#include "pastel/sys/rational/rational_real.hpp"
#include "pastel/sys/rational/rational_simplify.hpp"
#include "pastel/sys/rational/rational_stream.h"

#endif
