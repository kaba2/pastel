// Description: Digits for varying number bases

#ifndef PASTELSYS_DIGITS_H
#define PASTELSYS_DIGITS_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	inline char digit(integer i)
	{
		static PASTEL_CONSTEXPR char digitSet[] =
		{
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 
			'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 
			'u', 'v', 'w', 'x', 'y', 'z'
		};
		static PASTEL_CONSTEXPR integer maxBase = 
			sizeof(digitSet) / sizeof(char);

		PENSURE_OP(i, >=, 0);
		PENSURE_OP(i, <, maxBase);

		return digitSet[i];
	}

}

#endif
