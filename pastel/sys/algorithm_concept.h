// Description: Algorithm concept
// Documentation: algorithm.txt

#ifndef PASTEL_ALGORITHM_CONCEPT_H
#define PASTEL_ALGORITHM_CONCEPT_H

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
