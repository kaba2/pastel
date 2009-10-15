// Description: AcceptPoint concept

#ifndef PASTEL_ACCEPTPOINT_CONCEPT_H
#define PASTEL_ACCEPTPOINT_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	class AcceptPoint_Concept
	{
	public:
		typedef UserDefinedType Object;

		//! Return whether to accept an object.
		bool operator()(const Object&) const;
	};

}

#endif
