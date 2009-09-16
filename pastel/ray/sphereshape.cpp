#include "pastel/ray/sphereshape.h"

#include "pastel/geometry/intersect_ray_sphere.h"

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
			localGeometry.normal_ = localGeometry.position_;
			localGeometry.dpDu_ =
				extend(cross(shrink(localGeometry.position_)), 0);
			localGeometry.dpDv_ =
				cross(localGeometry.normal_, localGeometry.dpDu_);

			Vector3 ddpDuu;
			Vector3 ddpDuv;
			Vector3 ddpDvv;

			/*
			computeNormalDerivatives(
				localGeometry.dpDu_, localGeometry.dpDv_, localGeometry.normal_,
				ddpDuu, ddpDuv, ddpDvv,
				localGeometry.dnDu_, localGeometry.dnDv_);
			*/

			return true;
		}

		return false;
	}

}
