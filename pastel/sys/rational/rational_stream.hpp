#ifndef PASTELSYS_RATIONAL_STREAM_HPP
#define PASTELSYS_RATIONAL_STREAM_HPP

#include "pastel/sys/rational/rational_stream.h"

namespace Pastel
{

	template <typename Integer>
	std::ostream& operator<<(std::ostream& stream,
		const Rational<Integer>& number)
	{
		stream << number.m();
		if (number.n() != 1)
		{
			stream << "/" << number.n();
		}

		return stream;
	}

}

#endif
