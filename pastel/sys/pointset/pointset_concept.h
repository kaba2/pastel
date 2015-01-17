// Description: Point-set concept
// Documentation: pointset.txt

#ifndef PASTELSYS_POINTSET_CONCEPT_H
#define PASTELSYS_POINTSET_CONCEPT_H

#include "pastel/sys/input_concept.h"
#include "pastel/sys/point_concept.h"
#include "pastel/sys/location_set.h"

namespace Pastel
{

	namespace PointSet_Concept
	{

		// A model of the PointSet concept is either 
		// * a model of the Input, with Point elements, or
		// * a LocationSet, which pairs an Input with a Locator.

	}

}

#include "pastel/sys/pointset_dimension.h"
#include "pastel/sys/pointset_empty.h"
#include "pastel/sys/pointset_get.h"
#include "pastel/sys/pointset_input.h"
#include "pastel/sys/pointset_locator.h"
#include "pastel/sys/pointset_location.h"
#include "pastel/sys/pointset_point.h"
#include "pastel/sys/pointset_pop.h"
#include "pastel/sys/pointset_real.h"

#endif