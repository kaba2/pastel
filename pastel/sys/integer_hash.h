// Description: Hashing for fixed-sized integers

#ifndef PASTELSYS_INTEGER_HASH_H
#define PASTELSYS_INTEGER_HASH_H

#include "pastel/sys/integer.h"
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
