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
			// To compute the parameters and derivatives
			// we could simply use the functions
			// cartesianToSpherical, sphericalToCartesian, and
			// sphericalToCartesianDerivative. However, this would
			// miss a lot of optimization opportunities.
			// So let us do the computations by hand:

			// Let
			// u' = u * pi (u in [0, 1[)
			// v' = v * 2pi (v in [0, 1[)

			// p(u, v):
			// x = r cos(u')
			// y = r sin(u') cos(v')
			// z = r sin(u') sin(v')

			// (dp / du)(u, v):
			// x = -r sin(u') * pi
			// y = r cos(u') cos(v') * pi
			// z = r cos(u') sin(v') * pi
			
			// (d^2 / du^2)(u, v):
			// x = -r cos(u') * pi^2
			// y = -r sin(u') cos(v') * pi^2
			// z = -r sin(u') sin(v') * pi^2

			// (dp / dv)(u, v):
			// x = 0
			// y = -r sin(u') sin(v') * 2pi
			// z = r sin(u') cos(v') * 2pi

			// (d^2p / dv^2)(u, v):
			// x = 0
			// y = -r sin(u') cos(v') * 4pi^2
			// z = -r sin(u') sin(v') * 4pi^2

			// (d^2p / (du dv))(u, v):
			// x = 0
			// y = -r cos(u') sin(v') * 2pi^2
			// z = r cos(u') cos(v') * 2pi^2

			// u' = acos(x / r)
			// v' = atan2(z, y)

			surface.position = ray.at(hitList[0]);
			surface.normal = 
				surface.position * sphere_.inverseRadius();

			const real pi = constantPi<real>();
			const real piSquared = square(pi);

			const real rPi = sphere_.radius() * pi;
			real u = std::acos(
				surface.position.x() * sphere_.inverseRadius());
			real v = std::atan2(
				surface.position.z(), surface.position.y());
			if (v < 0)
			{
				v += 2 * pi;
			}

			const real uSin = std::sin(u);
			const real uCos = std::cos(u);

			const real vSin = std::sin(v);
			const real vCos = std::cos(v);

			const real a = rPi * uCos * vCos;
			const real b = rPi * uCos * vSin;
			const real c = rPi * uSin * vCos;
			const real d = rPi * uSin * vSin;

			surface.dpDu.set(
				-rPi * uSin, a, b);
			surface.dpDv.set(
				0, -2 * d, 2 * c);

			surface.ddpDuu = -surface.position * piSquared;
			surface.ddpDvv.set(
				0, 
				surface.ddpDuu.y() * 4,
				surface.ddpDuu.z() * 4);
			surface.ddpDuv.set(0, -2 * pi * b, 2 * pi * a);

			surface.q.set(u / pi, v / (2 * pi));

			tClosest = hitList[0];

			return true;
		}

		return false;
	}

	AlignedBox3 Sphere_Shape::bound() const
	{
		const AlignedBox3 result(
			Vector3(-sphere_.radius()), 
			Vector3(sphere_.radius()));

		return result;
	}

}
