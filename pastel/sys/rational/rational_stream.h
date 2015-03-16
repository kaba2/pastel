// Description: Print a rational number to a stream

#ifndef PASTELSYS_RATIONAL_STREAM_H
#define PASTELSYS_RATIONAL_STREAM_H

#include "pastel/sys/rational/rational.h"

#include <iostream>

namespace Pastel
{

	//! Prints a rational number to a stream.
	template <typename Integer>
	std::ostream& operator<<(
		std::ostream& stream,
		const Rational<Integer>& number);

}

#include "pastel/sys/rational/rational_stream.hpp"

#endif
