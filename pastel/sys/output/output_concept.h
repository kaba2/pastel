// Description: Output concept
// Documentation: output.txt

#ifndef PASTELSYS_OUTPUT_CONCEPT_H
#define PASTELSYS_OUTPUT_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename T, typename... Arguments>
	concept Output_Concept_ = requires(T t, Arguments... arguments) {
		//! Reports 'that'.
		{t(arguments...)};
	};

	template <typename T, typename... Arguments>
	concept Output_Concept = 
		Output_Concept_<RemoveCvRef<T>, RemoveCvRef<Arguments>...>;

}

#include "pastel/sys/output/output_archetype.h"

#endif
