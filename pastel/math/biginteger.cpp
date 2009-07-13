#include "pastel/math/biginteger.h"

#include <limits>
#include <sstream>
#include <cctype>

namespace Pastel
{

	namespace
	{

		void trim(std::vector<uint16>& digits)
		{
			// Trim the result such that
			// there are no leading zeros.

			const integer digitsSize = digits.size();

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

	BigInteger::BigInteger()
		: digits_()
		, sign_(true)
	{
	}

	BigInteger::BigInteger(const std::string& text)
		: digits_()
		, sign_(true)
	{
		std::stringstream stream;
		stream << text;

		std::string number;
		stream >> number;

		const integer numberSize = number.size();

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

	BigInteger::BigInteger(integer that)
		: digits_()
		, sign_(that >= 0)
	{
		if (that != 0)
		{
			if (!sign_)
			{
				that = -that;
			}

			digits_.push_back(that & 0xFFFF);
			const integer upperBits = that >> 16;
			if (upperBits != 0)
			{
				digits_.push_back(upperBits);
			}
		}
	}

	BigInteger& BigInteger::operator=(const BigInteger& that)
	{
		BigInteger copy(that);
		swap(copy);
		return *this;
	}

	void BigInteger::swap(BigInteger& that)
	{
		digits_.swap(that.digits_);
		std::swap(sign_, that.sign_);
	}

	integer BigInteger::size() const
	{
		return digits_.size();
	}

	bool BigInteger::positive() const
	{
		return sign_ && !zero();
	}

	bool BigInteger::negative() const
	{
		return !sign_ && !zero();
	}

	bool BigInteger::zero() const
	{
		return digits_.empty();
	}

	void BigInteger::setDigit(integer index,
		uint16 value)
	{
		const integer thisSize = digits_.size();

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


	uint16 BigInteger::digit(integer index) const
	{
		PENSURE2(index >= 0 && index < digits_.size(), index,
			digits_.size());

		return digits_[index];
	}

	integer BigInteger::asNative() const
	{
		const integer thisSize = size();

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

	BigInteger& BigInteger::operator&=(const BigInteger& that)
	{
		const integer thisSize = size();
		const integer thatSize = that.size();
		const integer minSize = std::min(thisSize, thatSize);
		digits_.resize(minSize);

		for (integer i = 0;i < minSize;++i)
		{
			digits_[i] &= that.digits_[i];
		}

		trim(digits_);

		return *this;
	}

	BigInteger BigInteger::operator&(const BigInteger& that) const
	{
		BigInteger result(*this);
		result &= that;
		return result;
	}

	BigInteger& BigInteger::operator|=(const BigInteger& that)
	{
		const integer thisSize = size();
		const integer thatSize = that.size();
		const integer minSize = std::min(thisSize, thatSize);

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

	BigInteger BigInteger::operator|(const BigInteger& that) const
	{
		BigInteger result(*this);
		result |= that;
		return result;
	}

	BigInteger& BigInteger::operator++()
	{
		*this += 1;
		return *this;
	}

	BigInteger& BigInteger::operator--()
	{
		*this -= 1;
		return *this;
	}

	BigInteger BigInteger::operator+() const
	{
		return *this;
	}

	BigInteger BigInteger::operator-() const
	{
		BigInteger copy(*this);
		if (!copy.zero())
		{
			copy.sign_ = !copy.sign_;
		}
		return copy;
	}

	BigInteger BigInteger::operator+(const BigInteger& that) const
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

	BigInteger& BigInteger::operator+=(const BigInteger& that)
	{
		BigInteger copy(*this + that);
		swap(copy);
		return *this;
	}

	BigInteger& BigInteger::operator%=(const BigInteger& that)
	{
		BigInteger copy(*this % that);
		swap(copy);
		return *this;
	}

	BigInteger BigInteger::operator%(const BigInteger& that) const
	{
		BigInteger absThis(*this);
		absThis.sign_ = true;
		BigInteger absThat(that);
		absThat.sign_ = true;

		return absThis - (absThis / absThat) * absThat;
	}

	BigInteger BigInteger::operator-(const BigInteger& that) const
	{
		return BigInteger(*this + (-that));
	}

	BigInteger& BigInteger::operator-=(const BigInteger& that)
	{
		BigInteger copy(*this - that);
		swap(copy);
		return *this;
	}

	bool BigInteger::operator==(const BigInteger& that) const
	{
		return sign_ == that.sign_ && absoluteEqual(that);
	}

	bool BigInteger::operator<(const BigInteger& that) const
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

	BigInteger& BigInteger::operator*=(const BigInteger& that)
	{
		if (zero() || that.zero())
		{
			digits_.clear();
			sign_ = true;
			return *this;
		}

		const integer thatSize = that.size();

		BigInteger result;
		for (integer i = 0;i < thatSize;++i)
		{
			BigInteger digitProduct(*this);
			digitProduct.multiply(that.digits_[i]);
			digitProduct.shiftDigitsLeft(i);
			result += digitProduct;
		}

		result.sign_ = !(negative() ^ that.negative());
		swap(result);

		return *this;
	}

	BigInteger BigInteger::operator*(const BigInteger& that) const
	{
		BigInteger result(*this);
		result *= that;
		return result;
	}

	void BigInteger::multiply(uint16 that)
	{
		const integer thisSize = size();

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

	BigInteger& BigInteger::operator/=(const BigInteger& that)
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

			middle.sign_ = !(negative() ^ that.negative());

			swap(middle);
		}

		return *this;
	}

	BigInteger BigInteger::operator/(const BigInteger& that) const
	{
		BigInteger result(*this);
		result /= that;
		return result;
	}

	BigInteger& BigInteger::operator<<=(integer byBits)
	{
		ENSURE_OP(byBits, >=, 0);

		if (byBits == 0 ||
			zero())
		{
			return *this;
		}

		const integer byDigits = byBits >> 4;
		const integer byOffsetBits = byBits & 0xF;

		BigInteger result(*this);
		result.shiftDigitsLeft(byDigits);

		if (byOffsetBits > 0)
		{
			const integer resultSize = result.size();

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

	BigInteger BigInteger::operator<<(integer byBits) const
	{
		BigInteger result(*this);
		result <<= byBits;
		return result;
	}

	BigInteger& BigInteger::operator>>=(integer byBits)
	{
		ENSURE_OP(byBits, >=, 0);

		if (byBits == 0 ||
			zero())
		{
			return *this;
		}

		const integer byDigits = byBits >> 4;
		const integer byOffsetBits = byBits & 0xF;
		const integer mask = (1 << byOffsetBits) - 1;
		const integer carryOffset = 16 - byOffsetBits;

		BigInteger result(*this);
		result.shiftDigitsRight(byDigits);

		if (byOffsetBits > 0)
		{
			const integer resultSize = result.size();

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

	BigInteger BigInteger::operator>>(integer byBits) const
	{
		BigInteger result(*this);
		result >>= byBits;
		return result;
	}

	// Private

	BigInteger::BigInteger(DigitContainer& digits,
		bool sign)
		: digits_()
		, sign_(sign)
	{
		// Move semantics
		digits_.swap(digits);
	}

	BigInteger::BigInteger(int32 value,
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
		const uint16 upperBits = (uint16)(value >> 16);
		if (upperBits != 0)
		{
			digits_.push_back(upperBits);
		}
	}

	bool BigInteger::absoluteEqual(const BigInteger& that) const
	{
		const integer thisSize = size();
		const integer thatSize = that.size();

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

	bool BigInteger::absoluteLessThan(const BigInteger& that) const
	{
		if (size() < that.size())
		{
			return true;
		}
		else if (size() > that.size())
		{
			return false;
		}

		const integer thisSize = size();

		for (integer i = thisSize - 1; i >= 0;--i)
		{
			if (digits_[i] != that.digits_[i])
			{
				return digits_[i] < that.digits_[i];
			}
		}

		return false;
	}

	void BigInteger::add(const DigitContainer& left,
		const DigitContainer& right,
		DigitContainer& result) const
	{
		ASSERT(left.size() >= right.size());

		const integer leftSize = left.size();
		const integer rightSize = right.size();

		const uint32 MAX_UINT16 = 65535;

		result.clear();
		result.reserve(leftSize + 1);

		uint32 carry = 0;

		for (integer i = 0;i < rightSize;++i)
		{
			uint32 digitAdd = left[i];
			digitAdd += right[i];
			digitAdd += carry;

			if (digitAdd > MAX_UINT16)
			{
				result.push_back(
					(uint16)(digitAdd - (MAX_UINT16 + 1)));
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

			if (digitAdd > MAX_UINT16)
			{
				result.push_back(
					(uint16)(digitAdd - (MAX_UINT16 + 1)));
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

	void BigInteger::subtract(const DigitContainer& left,
		const DigitContainer& right,
		DigitContainer& result) const
	{
		ASSERT(left.size() >= right.size());

		if (right.empty())
		{
			result = left;
			return;
		}

		const integer leftSize = left.size();
		const integer rightSize = right.size();

		const int32 MAX_UINT16 = 65535;

		result = left;

		for (integer i = 0;i < rightSize;++i)
		{
			int32 digit = left[i];
			digit -= right[i];

			if (digit < 0)
			{
				integer nextNonZero = i + 1;
				while (nextNonZero < leftSize &&
					result[nextNonZero] == 0)
				{
					++nextNonZero;
				}
				ASSERT(result[nextNonZero] != 0);
				--result[nextNonZero];
				for (integer j = i + 1;j < nextNonZero;++j)
				{
					result[j] = MAX_UINT16;
				}

				result[i] =
					(uint16)(digit + (MAX_UINT16 + 1));
			}
			else
			{
				result[i] = (uint16)digit;
			}
		}

		trim(result);
	}

	void BigInteger::shiftDigitsLeft(integer byDigits)
	{
		ENSURE_OP(byDigits, >=, 0);

		if (byDigits == 0 ||
			zero())
		{
			return;
		}

		const integer thisSize = digits_.size();

		DigitContainer result;
		result.reserve(thisSize + byDigits);
		result.resize(byDigits, 0);

		for (integer i = 0;i < thisSize;++i)
		{
			result.push_back(digits_[i]);
		}

		digits_.swap(result);
	}

	void BigInteger::shiftDigitsRight(integer byDigits)
	{
		ENSURE_OP(byDigits, >=, 0);

		if (byDigits == 0 ||
			zero())
		{
			return;
		}

		const integer thisSize = digits_.size();

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
