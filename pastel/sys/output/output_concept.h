// Description: Output concept
// Documentation: output.txt

#ifndef PASTELSYS_OUTPUT_CONCEPT_H
#define PASTELSYS_OUTPUT_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	struct Output_Concept
	{
		template <
			typename Type,
			typename... ArgumentSet>
		auto requires_(
			Type&& t, 
			ArgumentSet&&... argumentSet) -> decltype
		(
			conceptCheck(
				//! Reports 'that'.
				(t(argumentSet...), 0)
			)
		);
	};

	template <typename T, typename... Arguments>
	concept Output_Concept_ = requires(T t, Arguments... arguments) {
		//! Reports 'that'.
		{t(arguments...)};
	};

}

#include "pastel/sys/output/output_archetype.h"

#endif
