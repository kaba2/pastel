// Description: BigInteger class
// Detail: Arbitrary precision integer modeling the Integer concept.

#ifndef PASTEL_BIGINTEGER_H
#define PASTEL_BIGINTEGER_H

#include "pastel/sys/integer_concept.h"
#include "pastel/sys/real_concept.h"
#include "pastel/sys/mytypes.h"

#include "pastel/math/mathlibrary.h"

#include <vector>
#include <string>

#include <boost/operators.hpp>

namespace Pastel
{

	class PASTELSYS BigInteger
		: boost::less_than_comparable<
		BigInteger
		, boost::equality_comparable<
		BigInteger
		, boost::incrementable<
		BigInteger
		, boost::decrementable<
		BigInteger
		> > > >
	{
	public:
		class BadNumberString {};

		// Using default copy constructor.
		// Using default destructor.

		BigInteger();
		explicit BigInteger(const std::string& text);
		// Implicit conversion allowed.
		BigInteger(integer that);

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

#include "pastel/sys/biginteger_tools.h"

#include "pastel/sys/biginteger_more.h"

#endif
