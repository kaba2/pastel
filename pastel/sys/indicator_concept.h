// Description: Indicator concept

#ifndef PASTELSYS_INDICATOR_CONCEPT_H
#define PASTELSYS_INDICATOR_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace Indicator_Concept
	{

		class Indicator
		{
		public:
			typedef UserDefinedType Point;

			//! Return whether to accept an object.
			bool operator()(const Point& point) const;
		};

	}

}

#endif
