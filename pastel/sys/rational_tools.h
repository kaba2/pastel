// Description: Algorithms for rational numbers
// Detail: Pretty printing to a stream, etc.

#ifndef PASTELSYS_RATIONAL_TOOLS_H
#define PASTELSYS_RATIONAL_TOOLS_H

#include "pastel/sys/rational.h"

#include "pastel/sys/constants.h"

#include <iostream>

namespace Pastel
{

	template <typename Integer>
	std::ostream& operator<<(std::ostream& stream,
		const Rational<Integer>& number);

}

#include "pastel/sys/rational_tools.hpp"

#endif
