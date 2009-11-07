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

		return intersect_(transformedRay, surface, tClosest);
	}

	void Thing::setTransformation(
		const AffineTransformation3& worldTransformation)
	{
		transformation_ = worldTransformation;
	}

	const AffineBijection3& Thing::transformation() const
	{
		return transformation_;
	}

}
