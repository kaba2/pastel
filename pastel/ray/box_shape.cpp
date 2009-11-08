#include "pastel/ray/box_shape.h"

#include "pastel/geometry/intersect_ray_alignedbox.h"

namespace Pastel
{

	Box_Shape::Box_Shape()
		: radius_(1)
	{
	}

	Box_Shape::Box_Shape(const Vector3& radius)
		: radius_(radius)
	{
		ENSURE(allGreaterEqual(radius, 0));
	}

	Box_Shape::~Box_Shape()
	{
	}

	bool Box_Shape::intersect(
		const Ray3& ray,
		LocalGeometry& surface,
		real& tClosest) const
	{
		Vector2 hitList;

		if (Pastel::intersect(ray, AlignedBox3(-radius_, radius_), hitList))
		{
			surface.position = ray.at(hitList[0]);
			surface.normal = surface.position;
			surface.dpDu =
				extend(cross(shrink(surface.position)), 0);
			surface.dpDv =
				cross(surface.normal, surface.dpDu);
			surface.ddpDuu.set(0);
			surface.ddpDuv.set(0);
			surface.ddpDvv.set(0);

			tClosest = hitList[0];

			return true;
		}

		return false;
	}

	AlignedBox3 Box_Shape::bound() const
	{
		const AlignedBox3 result(
			-radius_, radius_);

		return result;
	}

}
