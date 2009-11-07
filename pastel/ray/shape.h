#ifndef PASTEL_SHAPE_H
#define PASTEL_SHAPE_H

#include "pastel/sys/countedptr.h"

#include "pastel/ray/localgeometry.h"

#include "pastel/geometry/ray.h"

namespace Pastel
{

	class PASTELRAY Shape
		: public ReferenceCounted
	{
	public:
		virtual ~Shape()
		{
		}

		virtual bool intersect(
			const Ray3& ray,
			LocalGeometry& surface,
			real& tClosest) const = 0;
	};

}

#endif
