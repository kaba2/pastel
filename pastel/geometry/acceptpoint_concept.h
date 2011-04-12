// Description: AcceptPoint concept

#ifndef PASTEL_ACCEPTPOINT_CONCEPT_H
#define PASTEL_ACCEPTPOINT_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace AcceptPoint_Concept
	{

		class AcceptPoint
		{
		public:
			typedef UserDefinedType Point;

			//! Return whether to accept an object.
			bool operator()(const Point& point) const;
		};

	}

}

#endif
