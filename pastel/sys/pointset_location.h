// Description: Location of a point-set

#ifndef PASTELSYS_POINTSET_LOCATION_H
#define PASTELSYS_POINTSET_LOCATION_H

#include "pastel/sys/location.h"
#include "pastel/sys/pointset_point.h"
#include "pastel/sys/pointset_locator.h"

namespace Pastel
{

	template <typename PointSet>
	using PointSet_Location = 
		Location<
			PointSet_Point<PointSet>, 
			PointSet_Locator<PointSet>
		>;

}

#endif
