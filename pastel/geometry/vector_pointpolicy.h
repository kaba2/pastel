// Description: Vector_PointPolicy class
// Detail: Points represented by the Vector class.
// Documentation: pointpolicy.txt

#ifndef PASTEL_VECTOR_POINTPOLICY_H
#define PASTEL_VECTOR_POINTPOLICY_H

#include "pastel/geometry/pointpolicy_concept.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real, int N>
	class Vector_PointPolicy
	{
	public:
		typedef Real Coordinate;
		typedef Vector<Real, N> Object;

		const Coordinate* point(const Object& object) const
		{
			return object.rawBegin();
		}

		const Coordinate& point(const Object& object, integer axis) const
		{
			return object[axis];
		}
	};

}

#endif
