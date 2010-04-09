// Description: PointPolicy concept
// Documentation: pointpolicy.txt

#ifndef PASTEL_POINTPOLICY_CONCEPT_H
#define PASTEL_POINTPOLICY_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	class PointPolicy_Concept
	{
	public:
		// Defines the type of the coordinates.
		typedef UserDefinedType Coordinate;
		// Defines the type of stored object.
		typedef UserDefinedType Object;

		//! Returns a pointer to a coordinate array for the point.
		const Coordinate* point(const Object& object) const;

		//! Returns a coordinate of the given point on the given axis.
		Coordinate point(const Object& object, integer axis) const;
	};

}

#endif
