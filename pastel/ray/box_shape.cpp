#include "pastel/ray/box_shape.h"

#include "pastel/geometry/intersect_ray_alignedbox.h"

namespace Pastel
{

	Box_Shape::Box_Shape()
		: box_(Vector3(-1), Vector3(1))
	{
	}

	Box_Shape::Box_Shape(const Vector3& radius)
		: box_(Vector3(-radius), Vector3(radius))
	{
		ENSURE(allGreaterEqual(radius, 0));
	}

	Box_Shape::~Box_Shape()
	{
	}

	bool Box_Shape::intersect(
		const Ray3& ray,
		real& tClosest) const
	{
		Vector2 hitList;
		if (!Pastel::intersect(ray, box_, hitList))
		{
			return false;
		}

		tClosest = hitList[0];

		return true;
	}

	AlignedBox3 Box_Shape::bound() const
	{
		return box_;
	}

	void Box_Shape::setRadius(const Vector3& radius)
	{
		box_.set(-radius, radius);
	}

	const Vector3& Box_Shape::radius() const
	{
		return box_.max();
	}

}
