// Description: Location of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_LOCATION_H
#define PASTELSYS_POINTSET_LOCATION_H

#include "pastel/sys/locator/location.h"
#include "pastel/sys/pointset/pointset_point.h"
#include "pastel/sys/pointset/pointset_locator.h"

namespace Pastel
{

	template <typename PointSet>
	using PointSet_Location = 
		Location<
			PointSet_Point<PointSet>, 
			PointSet_Locator<PointSet>
		>;

	template <typename PointSet>
	using PointSet_Location_F = 
		Identity_F<PointSet_Location<PointSet>>;

}

#endif
