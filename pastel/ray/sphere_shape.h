// Description: Sphere shape

#ifndef PASTELRAY_SPHERE_SHAPE_H
#define PASTELRAY_SPHERE_SHAPE_H

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
			real& tClosest) const;

		virtual LocalGeometry localGeometry(
			const Vector3& position) const;

		virtual AlignedBox3 bound() const;
		virtual AlignedBox3 bound(
			const AffineTransformation3& transformation) const;

	private:
		Sphere3 sphere_;
	};

}

#endif
