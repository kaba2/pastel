// Description: Algorithm concept

#ifndef PASTEL_ALGORITHM_CONCEPT_H
#define PASTEL_ALGORITHM_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	class Algorithm_Concept
	{
	public:
		template <typename AnyNumberOfArguments>
		UserDefinedType operator()(
			const AnyNumberOfArguments& that);
	};

}

#endif
