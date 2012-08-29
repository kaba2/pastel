#ifndef PASTELSYS_RATIONAL_TOOLS_HPP
#define PASTELSYS_RATIONAL_TOOLS_HPP

#include "pastel/sys/rational_tools.h"

namespace Pastel
{

	template <typename Integer>
	std::ostream& operator<<(std::ostream& stream,
		const Rational<Integer>& number)
	{
		stream << number.numerator();
		if (number.denominator() != 1)
		{
			stream << "/" << number.denominator();
		}

		return stream;
	}

}

#endif
