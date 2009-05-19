#ifndef PASTEL_NATIVEINTEGER_TOOLS_HPP
#define PASTEL_NATIVEINTEGER_TOOLS_HPP

#include "pastel/sys/nativeinteger_tools.h"

namespace Pastel
{

	template <typename Type>
	std::ostream& operator<<(std::ostream& stream, const NativeInteger<Type>& that)
	{
		stream << that.data();
		return stream;
	}

}

#endif
