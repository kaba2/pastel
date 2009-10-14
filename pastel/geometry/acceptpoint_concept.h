// Description: AcceptPoint concept
// Documentation: acceptpoint.txt

#ifndef PASTEL_ACCEPTPOINT_CONCEPT_H
#define PASTEL_ACCEPTPOINT_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	class AcceptPoint_Concept
	{
	public:
		typedef UserDefinedType Object;

		bool operator()(const Object&) const
		{
			return true;
		}
	};

}

#endif
