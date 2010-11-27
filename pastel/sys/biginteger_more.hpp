#ifndef PASTEL_BIGINTEGER_MORE_HPP
#define PASTEL_BIGINTEGER_MORE_HPP

#include "pastel/sys/biginteger_more.h"

namespace Pastel
{

	template <>
	class Real_Function<BigInteger>
	{
	public:
		static const bool Exists = true;

		const BigInteger& floor(const BigInteger& that)
		{
			return that;
		}

		const BigInteger& ceil(const BigInteger& that)
		{
			return that;
		}

		bool zero(const BigInteger& that)
		{
			return that.zero();
		}

		bool negative(const BigInteger& that)
		{
			return that.negative();
		}

		bool positive(const BigInteger& that)
		{
			return that.positive();
		}
	};

	inline void swap(BigInteger& left, BigInteger& right)
	{
		left.swap(right);
	}

}

#endif
