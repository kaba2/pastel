#ifndef PASTELRAY_SHAPE_H
#define PASTELRAY_SHAPE_H

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

		virtual bool intersect(const Ray3& ray,
			LocalGeometry& localGeometry) const = 0;
	};

	typedef CountedPtr<Shape> ShapeRef;
	typedef CountedPtr<const Shape> ConstShapeRef;

}

#endif
