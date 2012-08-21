// Description: Hash from hash() member function.
// Documentation: hash.txt

#ifndef PASTEL_EMBEDDED_HASH_H
#define PASTEL_EMBEDDED_HASH_H

#include "pastel/sys/hash.h"

namespace Pastel
{

	//! Computes hash from hash() member function.
	class Embedded_Hash
	{
	public:
		template <typename Type>
		hash_integer operator()(const Type& that) const
		{
			return that.hash();
		}
	};

}

#endif
