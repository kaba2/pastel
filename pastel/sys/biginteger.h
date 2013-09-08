// Description: BigInteger class
// Detail: Arbitrary precision integer modeling the Integer concept.

#ifndef PASTELSYS_BIGINTEGER_H
#define PASTELSYS_BIGINTEGER_H

#include "pastel/sys/integer_concept.h"
#include "pastel/sys/real_concept.h"
#include "pastel/sys/mytypes.h"

#include "pastel/math/mathlibrary.h"

#include <vector>
#include <string>
#include <type_traits>

#include <boost/operators.hpp>
#include <boost/mpl/if.hpp>

namespace Pastel
{

	class PASTELSYS BigInteger
		: boost::totally_ordered<BigInteger
		, boost::unit_steppable<BigInteger
		> >
	{
	public:
		class BadNumberString {};

		// Using default copy constructor.
		// Using default destructor.

		BigInteger();
		explicit BigInteger(const std::string& text);

		// Implicit conversion allowed.
		template <typename BuiltInInteger>
		BigInteger(BuiltInInteger that, 
			PASTEL_ENABLE_IF((std::is_integral<BuiltInInteger>), bool) visible = true);

		BigInteger& operator=(const BigInteger& that);

		void swap(BigInteger& that);

		integer size() const;

		bool positive() const;
		bool negative() const;
		bool zero() const;

		void setDigit(integer index, uint16 value);
		uint16 digit(integer index) const;

		integer asNative() const;

		BigInteger& operator&=(const BigInteger& that);
		BigInteger operator&(const BigInteger& that) const;

		BigInteger& operator|=(const BigInteger& that);
		BigInteger operator|(const BigInteger& that) const;

		BigInteger& operator++();
		BigInteger& operator--();

		BigInteger operator+() const;
		BigInteger operator-() const;
		BigInteger operator+(const BigInteger& that) const;
		BigInteger& operator+=(const BigInteger& that);
		BigInteger operator-(const BigInteger& that) const;
		BigInteger& operator-=(const BigInteger& that);
		BigInteger& operator*=(const BigInteger& that);
		BigInteger operator*(const BigInteger& that) const;
		BigInteger& operator/=(const BigInteger& that);
		BigInteger operator/(const BigInteger& that) const;

		BigInteger& operator%=(const BigInteger& that);
		BigInteger operator%(const BigInteger& that) const;

		BigInteger& operator<<=(integer byBits);
		BigInteger operator<<(integer byBits) const;
		BigInteger& operator>>=(integer byBits);
		BigInteger operator>>(integer byBits) const;

		bool operator<(const BigInteger& that) const;
		bool operator==(const BigInteger& that) const;

	private:
		typedef std::vector<uint16> DigitContainer;

		template <typename BuiltInInteger>
		void construct(
			PASTEL_ENABLE_IF((std::is_signed<BuiltInInteger>), BuiltInInteger) that);

		template <typename BuiltInInteger>
		void construct(
			PASTEL_DISABLE_IF((std::is_signed<BuiltInInteger>), BuiltInInteger) that);

		BigInteger(DigitContainer& digits,
			bool sign);
		BigInteger(int32 value,
			integer digitShift);

		void multiply(uint16 that);

		bool absoluteEqual(const BigInteger& that) const;
		bool absoluteLessThan(const BigInteger& that) const;
		void add(const DigitContainer& left,
			const DigitContainer& right,
			DigitContainer& result) const;
		void subtract(const DigitContainer& left,
			const DigitContainer& right,
			DigitContainer& result) const;
		void shiftDigitsLeft(integer byDigits);
		void shiftDigitsRight(integer byDigits);

		DigitContainer digits_;
		bool sign_;
	};

}

#include <iostream>

namespace Pastel
{

	PASTELSYS std::ostream& operator<<(
		std::ostream& stream,
		const BigInteger& number);


	void swap(BigInteger& left, BigInteger& right);

}

#include "pastel/sys/biginteger.hpp"

#endif
