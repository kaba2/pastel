// Description: Digits for varying number bases

#ifndef PASTELSYS_DIGITS_H
#define PASTELSYS_DIGITS_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	inline char integerAsDigit(integer i)
	{
		static constexpr char digitSet[] =
		{
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 
			'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 
			'u', 'v', 'w', 'x', 'y', 'z'
		};
		static constexpr integer maxBase = 
			sizeof(digitSet) / sizeof(char);

		PENSURE_OP(i, >=, 0);
		PENSURE_OP(i, <, maxBase);

		unused(maxBase);

		return digitSet[i];
	}

	inline integer digitAsInteger(char digit)
	{
		if (digit >= '0' && digit <= '9')
		{
			return digit - '0';
		}

		if (digit >= 'a' && digit <= 'z')
		{
			return 10 + (digit - 'a');
		}

		if (digit >= 'A' && digit <= 'Z')
		{
			return 10 + (digit - 'A');
		}

		bool knownDigit = false;
		ENSURE1(knownDigit, (integer)digit);

		return 0;
	}

}

#endif
