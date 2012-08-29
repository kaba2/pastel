#ifndef PASTELSYS_BIGINTEGER_HPP
#define PASTELSYS_BIGINTEGER_HPP

#include "pastel/sys/biginteger.h"

namespace Pastel
{

	template <typename BuiltInInteger>
	BigInteger::BigInteger(BuiltInInteger that, 
		PASTEL_ENABLE_IF((std::is_integral<BuiltInInteger>), bool) visible)
		: digits_()
		, sign_(that >= 0)
	{
		construct<BuiltInInteger>(that);
	}

	template <typename BuiltInInteger>
	void BigInteger::construct(
		PASTEL_ENABLE_IF((std::is_signed<BuiltInInteger>), BuiltInInteger) that)
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
		PASTEL_DISABLE_IF((std::is_signed<BuiltInInteger>), BuiltInInteger) that)
	{
		while(that != 0)
		{
			digits_.push_back(that & 0xFFFF);
			that >>= 16;
		}
	}

}

#endif
