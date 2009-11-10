#include "pastel/ray/sphere_shape.h"
#include "pastel/ray/differential_geometry.h"

#include "pastel/geometry/intersect_ray_sphere.h"
#include "pastel/geometry/bounding_alignedbox_sphere.h"

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
		real& tClosest) const
	{
		Vector2 hitList;
		if (!Pastel::intersect(ray, sphere_, hitList))
		{
			return false;
		}

		tClosest = hitList[0];
		return true;
	}

	LocalGeometry Sphere_Shape::localGeometry(
		const Vector3& position) const
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

		LocalGeometry surface;

		surface.position = position;
		surface.normal = 
			position * sphere_.inverseRadius();

		const real pi = constantPi<real>();
		const real piSquared = square(pi);

		const real rPi = sphere_.radius() * pi;
		real u = std::acos(
			position.x() * sphere_.inverseRadius());
		real v = std::atan2(
			position.z(), position.y());
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

		const Vector3 ddpDuu = 
			-position * piSquared;
		const Vector3 ddpDvv(
			0, 
			ddpDuu.y() * 4,
			ddpDuu.z() * 4);
		const Vector3 ddpDuv(
			0, -2 * pi * b, 2 * pi * a);

		computeNormalDerivatives(
			surface.normal,
			surface.dpDu,
			surface.dpDv,
			ddpDuu,
			ddpDuv,
			ddpDvv,
			surface.dnDu,
			surface.dnDv);

		surface.q.set(u / pi, v / (2 * pi));

		return surface;
	}

	AlignedBox3 Sphere_Shape::bound() const
	{
		return boundingAlignedBox(sphere_);
	}

	AlignedBox3 Sphere_Shape::bound(
		const AffineTransformation3& transformation) const
	{
		return boundingAlignedBox(
			sphere_, transformation);
	}

}
