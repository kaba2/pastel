#ifndef PASTEL_RATIONAL_TOOLS_HPP
#define PASTEL_RATIONAL_TOOLS_HPP

#include "pastel/math/rational_tools.h"

namespace Pastel
{

	template <typename Integer>
	std::ostream& operator<<(std::ostream& stream,
		const Rational<Integer>& number)
	{
		stream << number.numerator() << "/"
			<< number.denominator();

		return stream;
	}

}

#endif
