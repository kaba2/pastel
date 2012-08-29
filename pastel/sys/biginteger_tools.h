// Description: Algorithms for BigInteger's
// Detail: Pretty printing to a stream, etc.

#ifndef PASTELSYS_BIGINTEGER_TOOLS_H
#define PASTELSYS_BIGINTEGER_TOOLS_H

#include "pastel/sys/biginteger.h"

#include <iostream>

namespace Pastel
{

	PASTELSYS std::ostream& operator<<(
		std::ostream& stream,
		const BigInteger& number);

}

#endif
