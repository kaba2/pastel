// Description: Array_PointPolicy class
// Detail: Points represented by coordinate pointers.
// Documentation: pointpolicy.txt

#ifndef PASTEL_ARRAY_POINTPOLICY_H
#define PASTEL_ARRAY_POINTPOLICY_H

#include "pastel/geometry/pointpolicy_concept.h"

namespace Pastel
{

	template <typename Real>
	class Array_PointPolicy
	{
	public:
		typedef Real Coordinate;
		typedef const Coordinate* Object;

		const Coordinate* point(const Coordinate* object) const
		{
			return object;
		}

		const Coordinate& point(const Coordinate* object, integer axis) const
		{
			return object[axis];
		}
	};

}

#endif
