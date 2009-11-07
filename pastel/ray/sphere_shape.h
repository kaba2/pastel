#ifndef PASTEL_SPHERE_SHAPE_H
#define PASTEL_SPHERE_SHAPE_H

#include "pastel/ray/shape.h"

#include "pastel/geometry/sphere.h"

namespace Pastel
{

	class PASTELRAY Sphere_Shape
		: public Shape
	{
	public:
		explicit Sphere_Shape(real radius = 1);

		virtual ~Sphere_Shape();

		virtual bool intersect(
			const Ray3& ray,
			LocalGeometry& surface,
			real& tClosest) const;

	private:
		Sphere3 sphere_;
	};

}

#endif
