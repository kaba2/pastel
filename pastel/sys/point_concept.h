// Description: Point concept

#ifndef PASTELSYS_POINT_CONCEPT_H
#define PASTELSYS_POINT_CONCEPT_H

#include "pastel/sys/location.h"

namespace Pastel
{

	namespace Point_Concept
	{

		// A model of the Point concept is either 
		// * an arbitrary type, which has a default locator, or
		// * a Location, which pairs a point with a locator.

	}

	// The default locator can be specified for a new
	// point-type Point by specializing Default_Locator 
	// for const Point&. The specialization must be done
	// in the Pastel namespace. This is similar to how
	// extending std::hash works. The Constraint parameter
	// can be used for SFINAE constraints.
	template <
		typename Point,
		typename Constraint = void>
	class Default_Locator;

}

#include "pastel/sys/point_locator.h"
#include "pastel/sys/point_axis.h"
#include "pastel/sys/point_dimension.h"
#include "pastel/sys/point_point.h"
#include "pastel/sys/point_real.h"

#endif
