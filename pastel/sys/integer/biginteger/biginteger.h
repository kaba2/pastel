// Description: BigInteger class
// Detail: Arbitrary precision integer modeling the Integer concept.

#ifndef PASTELSYS_BIGINTEGER_H
#define PASTELSYS_BIGINTEGER_H

#include "pastel/sys/integer/integer_concept.h"
#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/mytypes.h"

#include <vector>
#include <string>
#include <type_traits>

#include <boost/operators.hpp>
#include <boost/mpl/if.hpp>

namespace Pastel
{

	class BigInteger
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
		template <typename BuiltInInteger, EnableIf<std::is_integral<BuiltInInteger>> = 0>
		BigInteger(BuiltInInteger that);

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
		using DigitContainer = std::vector<uint16>;

		template <
			typename BuiltInInteger,
			EnableIf<std::is_signed<BuiltInInteger>> = 0>
		void construct(BuiltInInteger that);

		template <
			typename BuiltInInteger,
			DisableIf<std::is_signed<BuiltInInteger>> = 0>
		void construct(BuiltInInteger that);

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

	std::ostream& operator<<(
		std::ostream& stream,
		const BigInteger& number);


	void swap(BigInteger& left, BigInteger& right);

}

#include "pastel/sys/integer/biginteger/biginteger.hpp"

#endif
