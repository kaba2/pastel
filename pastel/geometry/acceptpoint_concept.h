// Description: AcceptPoint concept

#ifndef PASTELGEOMETRY_ACCEPTPOINT_CONCEPT_H
#define PASTELGEOMETRY_ACCEPTPOINT_CONCEPT_H

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
