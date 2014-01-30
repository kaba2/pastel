// Description: Concepts for C-fast trie

#ifndef PASTELSYS_CFASTTRIE_CONCEPTS_H
#define PASTELSYS_CFASTTRIE_CONCEPTS_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace CFastTrie_Concepts
	{

		class Settings
		{
		public:
			static const integer Bits = UserDefinedInteger;
			using Value = UserDefinedType;
		};

	}

}

#endif
