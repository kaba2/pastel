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
		integer base = 10;

		if (stream.flags() & std::ios_base::hex)
		{
			base = 16;
		}
		else if (stream.flags() & std::ios_base::oct)
		{
			base = 8;
		}

		stream << that.asString(base);

		if (stream.flags() & std::ios_base::showbase)
		{
			stream << "_" << std::dec << base;
		}

		return stream;
	}

}

#endif
