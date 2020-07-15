// Description: Logical-not type-function
// Documentation: type_traits.txt

#ifndef PASTELSYS_NOT_H
#define PASTELSYS_NOT_H

#include "pastel/sys/type_traits/identity.h"

namespace Pastel
{

	template <typename Condition>
	using Not = std::bool_constant<!Condition::value>;

}

#endif
