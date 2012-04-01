#ifndef PASTEL_TREE_OPTIONS_CONCEPT_H
#define PASTEL_TREE_OPTIONS_CONCEPT_H

#include "pastel/sys/tree.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace Tree_Options_Concept
	{

		class Tree_Options
		{
		public:
			// The number of children in each node.
			enum {N = UserDefinedInteger};

			// The user data in each node.
			typedef UserDefinedType Data;
		};

	}

}

#endif
