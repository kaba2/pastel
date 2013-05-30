// Description: Concepts for YFastTrie

#ifndef PASTELSYS_YFASTTRIE_CONCEPTS_H
#define PASTELSYS_YFASTTRIE_CONCEPTS_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace YFastTrie_Concepts
	{

		class YFastTrie_Settings
		{
		public:
			//! The number of bits to use for keys.
			/*!
			Preconditions:
			Bits > 0
			isPowerOfTwo(Bits)
			*/
			static const integer Bits = UserDefinedInteger;

			//! The value-data to associate with each key.
			/*!
			Setting this to void ensures that no memory will
			be allocated to the value data.
			*/
			using Value = UserDefinedType;
		};

	}

}

#endif
