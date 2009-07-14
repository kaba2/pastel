// Description: Tools for working with BigInteger's
// Detail: pretty printing to a stream, etc.

#ifndef PASTEL_BIGINTEGER_TOOLS_H
#define PASTEL_BIGINTEGER_TOOLS_H

#include "pastel/math/biginteger.h"

#include <iostream>

namespace Pastel
{

	PASTELMATH std::ostream& operator<<(
		std::ostream& stream,
		const BigInteger& number);

}

#endif
