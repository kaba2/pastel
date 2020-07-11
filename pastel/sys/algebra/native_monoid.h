// Description: Native types as a monoid concept
// DocumentationOf: monoid_concept.h

#ifndef PASTELSYS_NATIVE_MONOID_H
#define PASTELSYS_NATIVE_MONOID_H

#include "pastel/sys/sfinae.h"

namespace Pastel
{

	// Additive monoid

	template <Arithmetic_Concept Type>
	bool zero(Type that)
	{
		return that == 0;
	}

	// Multiplicative monoid

	template <Arithmetic_Concept Type>
	bool one(Type that)
	{
		return that == 1;
	}

}

#endif
