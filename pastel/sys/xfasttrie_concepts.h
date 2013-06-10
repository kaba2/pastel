// Description: Concepts for X-fast trie

#ifndef PASTELSYS_XFASTTRIE_CONCEPTS_H
#define PASTELSYS_XFASTTRIE_CONCEPTS_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace XFastTrie_Concepts
	{

		class Settings
		{
		public:
			static const integer Bits = 64;
			using Value = UserDefinedType;
		};

	}

}

#endif
