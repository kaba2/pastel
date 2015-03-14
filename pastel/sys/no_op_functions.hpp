#ifndef PASTELSYS_NO_OP_FUNCTIONS_HPP
#define PASTELSYS_NO_OP_FUNCTIONS_HPP

#include "pastel/sys/no_op_functions.h"

namespace Pastel
{

	template <typename... Type>
	void unused(const Type&...)
	{
		// Do nothing.
	}

	template <typename Type>
	const Type& addConst(Type& that)
	{
		return (const Type&)that;
	}

	template <typename Type>
	Type& removeConst(const Type& that)
	{
		return (Type&)that;
	}

}

#endif
