// Description: Algorithm concept
// Documentation: algorithm_objects.txt

#ifndef PASTELSYS_ALGORITHM_CONCEPT_H
#define PASTELSYS_ALGORITHM_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace Algorithm_Concept
	{

		class Algorithm
		{
		public:
			template <typename AnyNumberOfArguments>
			UserDefinedType operator()(
				const AnyNumberOfArguments& that);
		};
	
	}

}

#endif
