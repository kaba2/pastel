#ifndef PASTELSYS_POINT_ARCHETYPE_H
#define PASTELSYS_POINT_ARCHETYPE_H

#include "pastel/sys/point/point_concept.h"

namespace Pastel
{

	struct Point_Archetype
	{
		integer pointDimension() const
		{
			return 0;
		}

		real pointAxis(integer i) const
		{
			return 0;
		}
	};

}

#endif
