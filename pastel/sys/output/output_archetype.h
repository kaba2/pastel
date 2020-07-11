// Description: Output archetype

#ifndef PASTELSYS_OUTPUT_ARCHETYPE_H
#define PASTELSYS_OUTPUT_ARCHETYPE_H

#include "pastel/sys/output/output_concept.h"

namespace Pastel
{

	class Output_Archetype
	{
	public:
		template <typename... TypeSet>
		void operator()(TypeSet&&...) const {}
	};

}

#endif
