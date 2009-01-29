#include "pastel/ray/sphereshape.h"

#include "pastel/geometry/intersect_ray_sphere.h"
#include "pastel/geometry/differential.h"

namespace Pastel
{

	SphereShape::~SphereShape()
	{
	}

	bool SphereShape::intersect(const Ray3& ray,
		LocalGeometry& localGeometry) const
	{
		Vector2 hitList;

		if (Pastel::intersect(ray, sphere_, hitList))
		{
			localGeometry.position_ = ray.at(hitList[0]);
			localGeometry.normal_ = asVector(localGeometry.position_);
			localGeometry.dpDu_ =
				extend(cross(shrink(asVector(localGeometry.position_))), 0);
			localGeometry.dpDv_ =
				cross(localGeometry.normal_, localGeometry.dpDu_);

			Vector3 ddpDuu;
			Vector3 ddpDuv;
			Vector3 ddpDvv;

			computeNormalDerivatives(
				localGeometry.dpDu_, localGeometry.dpDv_, localGeometry.normal_,
				ddpDuu, ddpDuv, ddpDvv,
				localGeometry.dnDu_, localGeometry.dnDv_);

			return true;
		}

		return false;
	}

}
