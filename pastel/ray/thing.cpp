#include "pastel/ray/thing.h"

namespace Pastel
{

	Thing::Thing()
		: transformation_()
	{
	}

	Thing::~Thing()
	{
	}

	bool Thing::intersect(
		const Ray3& ray,
		LocalGeometry& surface,
		real& tClosest) const
	{
		const Ray3 transformedRay(
			transformPoint(ray.position(), transformation_.inverse()),
			transformVector(ray.direction(), transformation_.inverse()));

		if (intersect_(transformedRay, surface, tClosest))
		{
			surface.position = transformPoint(surface.position, transformation_.forward());
			surface.normal = transformNormal(surface.normal, transformation_.inverse());
			surface.dpDu = transformVector(surface.dpDu, transformation_.forward());
			surface.dpDv = transformVector(surface.dpDv, transformation_.forward());
			surface.ddpDuu = transformVector(surface.ddpDuu, transformation_.forward());
			surface.ddpDuv = transformVector(surface.ddpDuv, transformation_.forward());
			surface.ddpDvv = transformVector(surface.ddpDvv, transformation_.forward());
			
			return true;
		}

		return false;
	}

	void Thing::setTransformation(
		const AffineTransformation3& transformation)
	{
		transformation_ = transformation;
	}

	const AffineBijection3& Thing::transformation() const
	{
		return transformation_;
	}

}
