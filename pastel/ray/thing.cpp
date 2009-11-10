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
		ShapeIntersection& shapeIntersection) const
	{
		const Ray3 transformedRay(
			transformPoint(ray.position(), transformation_.inverse()),
			transformVector(ray.direction(), transformation_.inverse()));

		if (intersect_(transformedRay, shapeIntersection))
		{
			shapeIntersection.shapeToWorld *= transformation_.forward();
			
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
