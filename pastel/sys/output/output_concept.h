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
			typename That>
		auto require(Type&& t, That&& that) -> decltype
		(
			conceptCheck(
				//! Reports 'that'.
				(t(that), 0)
			)
		);
	};

}

namespace Pastel
{

	class Output_Archetype
	{
	public:
		template <typename... TypeSet>
		void operator()(TypeSet&&...) {}
	};

}

#endif
