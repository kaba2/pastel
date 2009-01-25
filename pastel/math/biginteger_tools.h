#ifndef PASTELMATH_BIGINTEGER_TOOLS_H
#define PASTELMATH_BIGINTEGER_TOOLS_H

#include "pastel/math/biginteger.h"

#include <iostream>

namespace Pastel
{

	PASTELMATH std::ostream& operator<<(
		std::ostream& stream,
		const BigInteger& number);

}

#endif
