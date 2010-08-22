#ifndef PASTEL_BIGINTEGER_MORE_HPP
#define PASTEL_BIGINTEGER_MORE_HPP

#include "pastel/sys/biginteger_more.h"

namespace Pastel
{

	inline void swap(BigInteger& left, BigInteger& right)
	{
		left.swap(right);
	}

	inline bool zero(const BigInteger& that)
	{
		return that.zero();
	}

	inline bool negative(const BigInteger& that)
	{
		return that.negative();
	}

	inline bool positive(const BigInteger& that)
	{
		return that.positive();
	}

}

#endif
