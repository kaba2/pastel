#include "pastel/ray/sphere_shape.h"

#include "pastel/geometry/intersect_ray_sphere.h"

#include "pastel/math/coordinates.h"
#include "pastel/math/coordinates_derivatives.h"

namespace Pastel
{

	Sphere_Shape::Sphere_Shape(real radius)
		: sphere_(Vector3(0), radius)
	{
		ENSURE_OP(radius, >=, 0);
	}

	Sphere_Shape::~Sphere_Shape()
	{
	}

	bool Sphere_Shape::intersect(
		const Ray3& ray,
		LocalGeometry& surface,
		real& tClosest) const
	{
		Vector2 hitList;

		if (Pastel::intersect(ray, sphere_, hitList))
		{
			surface.position = ray.at(hitList[0]);
			surface.normal = 
				surface.position * sphere_.inverseRadius();
			
			const Vector3 spherical = 
				cartesianToSpherical(surface.position);

			surface.dpDu = sphericalToCartesianDerivative(
				spherical, Vector3i(0, 1, 0));
			surface.dpDv = sphericalToCartesianDerivative(
				spherical, Vector3i(0, 0, 1));

			surface.ddpDuu = sphericalToCartesianDerivative(
				spherical, Vector3i(0, 2, 0));
			surface.ddpDuv = sphericalToCartesianDerivative(
				spherical, Vector3i(0, 1, 1));
			surface.ddpDvv = sphericalToCartesianDerivative(
				spherical, Vector3i(0, 0, 2));

			surface.q = shrink(spherical, 0);
			//Vector2(mod(spherical[1], constantPi<real>()), mod(spherical[2], 2 * constantPi<real>()) / 2) / constantPi<real>();

			tClosest = hitList[0];

			return true;
		}

		return false;
	}

}
