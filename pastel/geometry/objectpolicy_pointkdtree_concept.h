// Description: ObjectPolicy_PointKdTree concept
// Documentation: pointkdtree.txt

#ifndef PASTEL_OBJECTPOLICY_POINTKDTREE_CONCEPT_H
#define PASTEL_OBJECTPOLICY_POINTKDTREE_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	class ObjectPolicy_PointKdTree_Concept
	{
	private:
		typedef UserDefinedType Real;

	public:
		// Defines the type of stored object.
		typedef UserDefinedType Object;

		//! Returns a pointer to a coordinate array for the point.
		const Real* point(const Object& object) const;

		//! Returns a coordinate of the given point on the given axis.
		Real point(const Object& object, integer axis) const;
	};

}

#endif
