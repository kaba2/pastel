// Description: Hashing for multi-integers

#ifndef PASTELSYS_MULTI_INTEGER_HASH_H
#define PASTELSYS_MULTI_INTEGER_HASH_H

#include "pastel/sys/multi_integer.h"
#include "pastel/sys/embedded_hash.h"

namespace std
{

	template <typename Integer_Settings>
	struct hash<Pastel::Integer<Integer_Settings>>
		: Pastel::Embedded_Hash
	{
	};

}

#endif
