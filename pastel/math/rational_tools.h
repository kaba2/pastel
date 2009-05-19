/*!
\file
\brief Tools for working with rational numbers.
*/

#ifndef PASTEL_RATIONAL_TOOLS_H
#define PASTEL_RATIONAL_TOOLS_H

#include "pastel/math/rational.h"

#include "pastel/sys/constants.h"

#include <iostream>

namespace Pastel
{

	template <typename Integer>
	std::ostream& operator<<(std::ostream& stream,
		const Rational<Integer>& number);

}

#include "pastel/math/rational_tools.hpp"

#endif
