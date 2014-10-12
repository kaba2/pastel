#ifndef PASTELSYS_BIGINTEGER_HPP
#define PASTELSYS_BIGINTEGER_HPP

#include "pastel/sys/biginteger.h"
#include "pastel/sys/ensure.h"

#include <limits>
#include <sstream>
#include <cctype>

namespace Pastel
{

	namespace
	{

		inline void trim(std::vector<uint16>& digits)
		{
			// Trim the result such that
			// there are no leading zeros.

			integer digitsSize = digits.size();

			if (digitsSize > 0)
			{
				integer lastNonZeroIndex = digitsSize - 1;


				while (lastNonZeroIndex >= 0 &&
					digits[lastNonZeroIndex] == 0)
				{
					--lastNonZeroIndex;
				}
				if (lastNonZeroIndex + 1 != digitsSize)
				{
					digits.resize(lastNonZeroIndex + 1);
				}
			}
		}

	}

	inline BigInteger::BigInteger()
		: digits_()
		, sign_(true)
	{
	}

	inline BigInteger::BigInteger(const std::string& text)
		: digits_()
		, sign_(true)
	{
		std::stringstream stream;
		stream << text;

		std::string number;
		stream >> number;

		integer numberSize = number.size();

		if (numberSize > 0)
		{
			integer offset = 0;
			if (number[offset] == '-')
			{
				sign_ = false;
				++offset;
			}
			else if (number[offset] == '+')
			{
				++offset;
			}

			while (offset < numberSize)
			{

				*this *= 10;
				if (!std::isdigit(number[offset]))
				{
					throw BadNumberString();
				}
				int32 digit = (int32)number[offset] - '0';

				*this += BigInteger(digit);
				++offset;
			}
		}
	}

	inline BigInteger& BigInteger::operator=(const BigInteger& that)
	{
		BigInteger copy(that);
		swap(copy);
		return *this;
	}

	inline void BigInteger::swap(BigInteger& that)
	{
		digits_.swap(that.digits_);
		std::swap(sign_, that.sign_);
	}

	inline integer BigInteger::size() const
	{
		return digits_.size();
	}

	inline bool BigInteger::positive() const
	{
		return sign_ && !zero();
	}

	inline bool BigInteger::negative() const
	{
		return !sign_ && !zero();
	}

	inline bool BigInteger::zero() const
	{
		return digits_.empty();
	}

	inline void BigInteger::setDigit(integer index,
		uint16 value)
	{
		integer thisSize = digits_.size();


		PENSURE2(index >= 0 && index < thisSize,
			index, thisSize);

		if (index == thisSize - 1 &&
			value == 0)
		{
			digits_.resize(thisSize - 1);
		}
		else
		{
			digits_[index] = value;
		}
	}


	inline uint16 BigInteger::digit(integer index) const
	{
		PENSURE2(index >= 0 && index < digits_.size(), index,
			digits_.size());

		return digits_[index];
	}

	inline integer BigInteger::asNative() const
	{
		integer thisSize = size();

		if (thisSize == 0)
		{
			return 0;
		}

		if (thisSize > 2 ||

			(thisSize == 2 &&
			(digits_[1] & 0x8000)))
		{
			if (negative())
			{
				return std::numeric_limits<integer>::min();
			}
			else
			{
				return std::numeric_limits<integer>::max();
			}
		}

		integer result = (integer)digits_[0];

		if (thisSize >= 2)
		{
			result += (((integer)(digits_[1] & 0x7FFF)) << 16);
		}

		if (negative())
		{
			result = -result;
		}

		return result;
	}

	inline BigInteger& BigInteger::operator&=(const BigInteger& that)
	{
		integer thisSize = size();
		integer thatSize = that.size();
		integer minSize = std::min(thisSize, thatSize);
		digits_.resize(minSize);

		for (integer i = 0;i < minSize;++i)
		{

			digits_[i] &= that.digits_[i];
		}

		trim(digits_);

		return *this;
	}

	inline BigInteger BigInteger::operator&(const BigInteger& that) const
	{
		BigInteger result(*this);
		result &= that;
		return result;
	}

	inline BigInteger& BigInteger::operator|=(const BigInteger& that)
	{
		integer thisSize = size();
		integer thatSize = that.size();
		integer minSize = std::min(thisSize, thatSize);

		if (thisSize < thatSize)
		{
			digits_.resize(thatSize, 0);
			for (integer i = thisSize;i < thatSize;++i)
			{
				digits_[i] = that.digits_[i];
			}
		}

		for (integer i = 0;i < minSize;++i)
		{
			digits_[i] |= that.digits_[i];
		}


		return *this;
	}

	inline BigInteger BigInteger::operator|(const BigInteger& that) const
	{
		BigInteger result(*this);
		result |= that;
		return result;
	}

	inline BigInteger& BigInteger::operator++()
	{
		*this += 1;
		return *this;
	}

	inline BigInteger& BigInteger::operator--()
	{
		*this -= 1;
		return *this;
	}

	inline BigInteger BigInteger::operator+() const
	{
		return *this;
	}

	inline BigInteger BigInteger::operator-() const
	{
		BigInteger copy(*this);
		if (!copy.zero())
		{
			copy.sign_ = !copy.sign_;
		}
		return copy;
	}

	inline BigInteger BigInteger::operator+(const BigInteger& that) const
	{
		DigitContainer result;
		bool thatIsAbsLess = that.absoluteLessThan(*this);
		bool resultSign = false;

		if (!negative())
		{
			if (!that.negative())
			{
				if (thatIsAbsLess)
				{
					// a + b = a + b
					add(digits_, that.digits_, result);
					resultSign = true;
				}
				else
				{
					// a + b = b + a
					add(that.digits_, digits_, result);
					resultSign = true;
				}
			}
			else
			{
				if (thatIsAbsLess)
				{
					// a + (-b) = a - b

					subtract(digits_, that.digits_, result);
					resultSign = true;
				}
				else
				{
					// a + (-b) = -(b - a)

					subtract(that.digits_, digits_, result);
					resultSign = false;
				}
			}
		}
		else
		{
			if (!that.negative())
			{
				if (thatIsAbsLess)
				{
					// (-a) + b = -(a - b)

					subtract(digits_, that.digits_, result);
					resultSign = false;
				}
				else
				{
					// (-a) + b = b - a
					subtract(that.digits_, digits_, result);
					resultSign = true;
				}
			}
			else
			{
				if (thatIsAbsLess)
				{
					// (-a) + (-b) = -(a + b)
					add(digits_, that.digits_, result);
					resultSign = false;
				}
				else
				{
					// (-a) + (-b) = -(b + a)
					add(that.digits_, digits_, result);
					resultSign = false;
				}
			}
		}

		if (result.empty())
		{
			resultSign = true;
		}
		return BigInteger(result, resultSign);
	}

	inline BigInteger& BigInteger::operator+=(const BigInteger& that)
	{
		BigInteger copy(*this + that);
		swap(copy);
		return *this;
	}

	inline BigInteger& BigInteger::operator%=(const BigInteger& that)
	{
		BigInteger copy(*this % that);
		swap(copy);
		return *this;
	}

	inline BigInteger BigInteger::operator%(const BigInteger& that) const
	{
		BigInteger absThis(*this);
		absThis.sign_ = true;
		BigInteger absThat(that);
		absThat.sign_ = true;

		return absThis - (absThis / absThat) * absThat;
	}

	inline BigInteger BigInteger::operator-(const BigInteger& that) const
	{
		return BigInteger(*this + (-that));
	}

	inline BigInteger& BigInteger::operator-=(const BigInteger& that)
	{
		BigInteger copy(*this - that);
		swap(copy);
		return *this;
	}

	inline bool BigInteger::operator==(const BigInteger& that) const
	{
		return sign_ == that.sign_ && absoluteEqual(that);
	}

	inline bool BigInteger::operator<(const BigInteger& that) const
	{
		if (negative())
		{
			if (that.negative())
			{
				return that.absoluteLessThan(*this);
			}
			else
			{
				return true;
			}
		}
		else
		{
			if (that.negative())
			{
				return false;
			}
		}

		return absoluteLessThan(that);
	}

	inline BigInteger& BigInteger::operator*=(const BigInteger& that)
	{
		if (zero() || that.zero())
		{
			digits_.clear();
			sign_ = true;
			return *this;
		}

		integer thatSize = that.size();

		BigInteger result;
		for (integer i = 0;i < thatSize;++i)
		{

			BigInteger digitProduct(*this);
			digitProduct.multiply(that.digits_[i]);
			digitProduct.shiftDigitsLeft(i);
			result += digitProduct;
		}

		result.sign_ = !(negative() != that.negative());
		swap(result);

		return *this;
	}

	inline BigInteger BigInteger::operator*(const BigInteger& that) const
	{
		BigInteger result(*this);
		result *= that;
		return result;
	}

	inline void BigInteger::multiply(uint16 that)
	{
		integer thisSize = size();

		BigInteger result;
		for (integer i = 0;i < thisSize;++i)
		{
			BigInteger productInteger(
				(int32)digits_[i] * (int32)that, i);
			result += productInteger;
		}

		result.sign_ = sign_;

		swap(result);
	}

	inline BigInteger& BigInteger::operator/=(const BigInteger& that)
	{
		ENSURE(!that.zero());

		if (absoluteLessThan(that))
		{
			*this = BigInteger();
			return *this;
		}

		if (!zero())
		{
			BigInteger minBound(0);
			BigInteger maxBound(*this);
			maxBound.sign_ = true;
			maxBound += BigInteger(1);

			BigInteger middle;

			while (true)
			{
				middle = (minBound + maxBound) >> 1;
				BigInteger result(middle * that);
				if (absoluteEqual(result) ||
					minBound >= maxBound - BigInteger(1))
				{
					break;
				}
				if (result.absoluteLessThan(*this))
				{
					minBound = middle;
				}
				else
				{
					maxBound = middle;
				}
			}

			middle.sign_ = !(negative() != that.negative());

			swap(middle);
		}

		return *this;
	}

	inline BigInteger BigInteger::operator/(const BigInteger& that) const
	{
		BigInteger result(*this);
		result /= that;
		return result;
	}

	inline BigInteger& BigInteger::operator<<=(integer byBits)
	{
		ENSURE_OP(byBits, >=, 0);

		if (byBits == 0 ||
			zero())
		{
			return *this;
		}

		integer byDigits = byBits >> 4;

		const integer byOffsetBits = byBits & 0xF;

		BigInteger result(*this);
		result.shiftDigitsLeft(byDigits);

		if (byOffsetBits > 0)
		{
			integer resultSize = result.size();

			uint32 carry = 0;
			for (integer i = 0;i < resultSize;++i)
			{
				uint32 digitAdd = result.digits_[i];
				digitAdd <<= byOffsetBits;
				digitAdd += carry;
				result.digits_[i] = digitAdd;
				carry = (digitAdd >> 16);
			}
			if (carry != 0)
			{
				result.digits_.push_back(carry);
			}
		}

		swap(result);


		return *this;
	}

	inline BigInteger BigInteger::operator<<(integer byBits) const
	{
		BigInteger result(*this);
		result <<= byBits;
		return result;
	}

	inline BigInteger& BigInteger::operator>>=(integer byBits)
	{
		ENSURE_OP(byBits, >=, 0);

		if (byBits == 0 ||
			zero())
		{
			return *this;
		}

		integer byDigits = byBits >> 4;

		const integer byOffsetBits = byBits & 0xF;
		integer mask = (1 << byOffsetBits) - 1;
		integer carryOffset = 16 - byOffsetBits;


		BigInteger result(*this);
		result.shiftDigitsRight(byDigits);

		if (byOffsetBits > 0)
		{
			integer resultSize = result.size();

			uint32 carry = 0;
			for (integer i = resultSize - 1;i >= 0;--i)
			{
				uint32 digitAdd = result.digits_[i];

				const uint32 newCarry = (digitAdd & mask) << carryOffset;
				digitAdd >>= byOffsetBits;
				digitAdd += carry;
				result.digits_[i] = digitAdd;
				carry = newCarry;
			}
		}

		trim(result.digits_);
		if (result.digits_.empty())
		{
			result.sign_ = true;
		}

		swap(result);

		return *this;
	}

	inline BigInteger BigInteger::operator>>(integer byBits) const
	{
		BigInteger result(*this);
		result >>= byBits;
		return result;
	}

	// Private

	inline BigInteger::BigInteger(DigitContainer& digits,
		bool sign)
		: digits_()
		, sign_(sign)
	{
		// Move semantics
		digits_.swap(digits);
	}

	inline BigInteger::BigInteger(int32 value,
		integer digitShift)
		: digits_()
		, sign_(value >= 0)
	{
		if (!sign_)
		{
			value = -value;
		}

		digits_.reserve(digitShift + 2);
		digits_.resize(digitShift, 0);
		digits_.push_back(value & 0xFFFF);
		uint16 upperBits = (uint16)(value >> 16);
		if (upperBits != 0)
		{
			digits_.push_back(upperBits);
		}
	}


	inline bool BigInteger::absoluteEqual(const BigInteger& that) const
	{
		integer thisSize = size();
		integer thatSize = that.size();

		if (thisSize != thatSize)
		{
			return false;
		}

		for (integer i = 0;i < thisSize;++i)
		{
			if (digits_[i] != that.digits_[i])
			{
				return false;
			}
		}

		return true;
	}

	inline bool BigInteger::absoluteLessThan(const BigInteger& that) const
	{
		if (size() < that.size())
		{
			return true;
		}
		else if (size() > that.size())
		{
			return false;
		}

		integer thisSize = size();

		for (integer i = thisSize - 1; i >= 0;--i)
		{
			if (digits_[i] != that.digits_[i])
			{
				return digits_[i] < that.digits_[i];
			}
		}

		return false;
	}

	inline void BigInteger::add(
		const DigitContainer& left,
		const DigitContainer& right,
		DigitContainer& result) const
	{
		ASSERT(left.size() >= right.size());

		integer leftSize = left.size();
		integer rightSize = right.size();

		uint32 MaxDigit = 65535;

		result.clear();
		result.reserve(leftSize + 1);

		uint32 carry = 0;

		for (integer i = 0;i < rightSize;++i)
		{
			uint32 digitAdd = left[i];
			digitAdd += right[i];
			digitAdd += carry;

			if (digitAdd > MaxDigit)
			{
				result.push_back(
					(uint16)(digitAdd - (MaxDigit + 1)));
				carry = 1;
			}
			else
			{
				result.push_back((uint16)digitAdd);
				carry = 0;
			}
		}

		for (integer i = rightSize;i < leftSize;++i)
		{
			uint32 digitAdd = left[i];
			digitAdd += carry;

			if (digitAdd > MaxDigit)
			{
				result.push_back(
					(uint16)(digitAdd - (MaxDigit + 1)));
				carry = 1;
			}
			else
			{
				result.push_back((uint16)digitAdd);
				carry = 0;
			}
		}

		if (carry == 1)
		{
			result.push_back(carry);
		}
	}

	inline void BigInteger::subtract(
		const DigitContainer& left,
		const DigitContainer& right,
		DigitContainer& result) const
	{
		// This function is only called when
		// right.absoluteLessThan(left).

		ASSERT(left.size() >= right.size());

		if (right.empty())
		{
			result = left;
			return;
		}

		integer leftSize = left.size();
		integer rightSize = right.size();

		int32 digits = 65536;
		int32 borrow = 0;

		result = left;

		for (integer i = 0;i < rightSize;++i)
		{
			int32 digit = 
				(int32)result[i] - 
				(int32)right[i] - 
				(int32)borrow;

			if (digit < 0)
			{
				result[i] =
					(uint16)(digit + digits);
				borrow = 1;
			}
			else
			{
				result[i] = (uint16)digit;
				borrow = 0;
			}
		}

		for (integer i = rightSize;i < leftSize;++i)
		{
			int32 digit = 
				(int32)result[i] - 
				(int32)borrow;

			if (digit < 0)
			{
				result[i] =
					(uint16)(digit + digits);
				borrow = 1;
			}
			else
			{
				result[i] = (uint16)digit;
				borrow = 0;
			}
		}

		ASSERT_OP(borrow, ==, 0);

		trim(result);
	}

	inline void BigInteger::shiftDigitsLeft(integer byDigits)
	{
		ENSURE_OP(byDigits, >=, 0);

		if (byDigits == 0 ||
			zero())
		{
			return;
		}

		integer thisSize = digits_.size();

		DigitContainer result;
		result.reserve(thisSize + byDigits);
		result.resize(byDigits, 0);

		for (integer i = 0;i < thisSize;++i)
		{
			result.push_back(digits_[i]);
		}

		digits_.swap(result);
	}

	inline void BigInteger::shiftDigitsRight(integer byDigits)
	{
		ENSURE_OP(byDigits, >=, 0);

		if (byDigits == 0 ||
			zero())
		{
			return;
		}

		integer thisSize = digits_.size();

		if (thisSize <= byDigits)
		{
			digits_.clear();
			return;
		}

		DigitContainer result;
		result.reserve(thisSize - byDigits);

		for (integer i = byDigits;i < thisSize;++i)
		{
			result.push_back(digits_[i]);
		}

		digits_.swap(result);
	}

}

#include <sstream>
#include <string>
#include <algorithm>

namespace Pastel
{


	inline std::ostream& operator<<(
		std::ostream& stream,
		const BigInteger& number)
	{
		if (number.zero())
		{
			stream << '0';
			return stream;
		}

		std::stringstream numberStream;
		BigInteger tempNumber(number);
		while (!tempNumber.zero())
		{
			BigInteger remainder(
				tempNumber % BigInteger(10));

			numberStream << remainder.asNative();

			tempNumber /= BigInteger(10);
		}

		if (number.negative())
		{
			numberStream << '-';
		}

		std::string numberText;
		numberStream >> numberText;

		std::reverse(numberText.begin(),
			numberText.end());

		stream << numberText;

		return stream;
	}

	template <typename BuiltInInteger>
	BigInteger::BigInteger(BuiltInInteger that, 
		EnableIf<std::is_integral<BuiltInInteger>, bool> visible)
		: digits_()
		, sign_(that >= 0)
	{
		construct<BuiltInInteger>(that);
	}

	template <typename BuiltInInteger>
	void BigInteger::construct(
		EnableIf<std::is_signed<BuiltInInteger>, BuiltInInteger> that)
	{
		if (!sign_) {that = -that;}
			
		while(that != 0)
		{
			digits_.push_back(that & 0xFFFF);
			that >>= 16;
		}
	}

	template <typename BuiltInInteger>
	void BigInteger::construct(
		DisableIf<std::is_signed<BuiltInInteger>, BuiltInInteger> that)
	{
		while(that != 0)
		{
			digits_.push_back(that & 0xFFFF);
			that >>= 16;
		}
	}

}

namespace Pastel
{

	inline void swap(BigInteger& left, BigInteger& right)
	{
		left.swap(right);
	}

	// Integer

	inline bool even(const BigInteger& that)
	{
		return (that.digit(0) & 1) == 0;
	}

	inline bool odd(const BigInteger& that)
	{
		return !even(that);
	}

	// Real (partial)

	inline const BigInteger& floor(const BigInteger& that)
	{
		return that;
	}

	inline const BigInteger& ceil(const BigInteger& that)
	{
		return that;
	}

	// Ordered additive monoid

	inline BigInteger abs(const BigInteger& that)
	{
		return that.positive() ? that : -that;
	}

	inline bool negative(const BigInteger& that)
	{
		return that.negative();
	}

	inline bool positive(const BigInteger& that)
	{
		return that.positive();
	}

	// Additive monoid

	inline bool zero(const BigInteger& that)
	{
		return that.zero();
	}

}

#endif
