#ifndef PASTELSYS_NATIVEINTEGER_TOOLS_H
#define PASTELSYS_NATIVEINTEGER_TOOLS_H

#include "pastel/sys/nativeinteger.h"

#include <iostream>

namespace Pastel
{

	template <typename Type>
	std::ostream& operator<<(std::ostream& stream, const NativeInteger<Type>& that);

}

#include "pastel/sys/nativeinteger_tools.hpp"

#endif
