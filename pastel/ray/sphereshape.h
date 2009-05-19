#ifndef PASTEL_SPHERESHAPE_H
#define PASTEL_SPHERESHAPE_H

#include "pastel/ray/shape.h"

#include "pastel/geometry/sphere.h"

namespace Pastel
{

	class PASTELRAY SphereShape
		: public Shape
	{
	public:
		virtual ~SphereShape();

		virtual bool intersect(const Ray3& ray,
			LocalGeometry& localGeometry) const;

	private:
		Sphere3 sphere_;
	};

}

#endif
