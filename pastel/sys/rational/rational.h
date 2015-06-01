// Description: Rational number
// Tags: C++11

#ifndef PASTELSYS_RATIONAL_H
#define PASTELSYS_RATIONAL_H

#include "pastel/sys/integer/integer_concept.h"
#include "pastel/sys/type_traits/and.h"
#include "pastel/sys/type_traits/or.h"
#include "pastel/sys/type_traits/not.h"

#include <boost/operators.hpp>

namespace Pastel
{

	namespace Rational_
	{

		template <
			typename Integer,
			typename That_Integer>
		using IsInteger =
			And<
				Not<std::is_integral<That_Integer>>, 
				std::is_same<That_Integer, Integer>
			>;

		template <
			typename Integer,
			typename That_Integer>
		using IsNativeOrInteger =
			Or<
				std::is_integral<That_Integer>,
				IsInteger<Integer, That_Integer>
			>;

	}

	enum class Rounding : integer
	{
		Truncate,
		RoundUp
	};

}

namespace Pastel
{

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

		PASTEL_CONCEPT_CHECK(Integer, Integer_Concept);

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
			Requires<Rational_::IsNativeOrInteger<Integer, That_Integer>> = 0>
		Rational(That_Integer wholes);

		//! Constructs with the value (m / n).
		template <
			typename M_Integer, 
			typename N_Integer,
			Requires<
				Rational_::IsNativeOrInteger<Integer, M_Integer>,
				Rational_::IsNativeOrInteger<Integer, N_Integer>
			> = 0
		>
		Rational(
			M_Integer m,
			N_Integer n);

		//! Constructs with a native floating point number.
		/*!
		Implicit conversion allowed.

		Preconditions:
		nMax >= 0

		maxError:
		Maximum allowed absolute error for the
		approximation.

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
			Requires<std::is_floating_point<Real>> = 0>
		Rational(
			Real that,
			Integer nMax = 0,
			Real maxError = 0);

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
			Requires<std::is_floating_point<Real>> = 0>
		Real asReal() const;

		//! Returns the number as a ratio of integers.
		std::string asStringRatio() const;

		//! Returns a b-base expansion of the number.
		/*!
		Preconditions:
		base >= 2
		digits >= 0

		Optional arguments
		------------------

		base (integer : 10):
		The base to use.

		showBase (bool : false):
		Whether to show the base.

		digits (integer : 3):
		The digits to show for the fractional part.

		shortenExact (bool : true):
		Whether to cut off trailing digits in case
		of an exact number.

		rounding (Rounding : Rouding::RoundUp):
		How to handle rounding.
		*/
		template <typename... ArgumentSet>
		std::string asString(
			ArgumentSet&&... argumentSet) const;

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

		//! Invert the 
		Rational<Integer>& invert()
		{
			using std::swap;
			
			swap(m_, n_);
			if (negative(n_))
			{
				// As an invariant, the divisor
				// is always positive. Make it so.
				m_ = -m_;
				n_ = -n_;
			}

			return *this;
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
