#ifndef PASTELSYS_MULTI_INTEGER_STREAM_HPP
#define PASTELSYS_MULTI_INTEGER_STREAM_HPP

#include "pastel/sys/integer/multi_integer/multi_integer.h"

#include <iostream>
#include <iomanip>

namespace Pastel
{

	template <typename Integer_Settings>
	std::ostream& operator<<(
		std::ostream& stream, 
		const MultiInteger<Integer_Settings>& that)
	{
		for (integer i = that.words() - 1;i >= 0;--i)
		{
			stream << std::setw(that.BitsInWord / 4)
				<< std::setfill('0')
				<< std::hex
				<< (uinteger)that.word(i);
		}

		return stream;
	}

}

#endif
