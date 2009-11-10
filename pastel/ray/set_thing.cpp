#include "pastel/ray/set_thing.h"
#include "pastel/ray/shape_thing.h"

#include "pastel/geometry/bounding_alignedbox_alignedbox.h"

namespace Pastel
{

	Set_Thing::~Set_Thing()
	{
	}

	AlignedBox3 Set_Thing::bound() const
	{
		return boundingAlignedBox(
			bound_,
			transformation().forward());
	}

	// Private

	bool Set_Thing::intersect_(
		const Ray3& ray,
		ShapeIntersection& shapeIntersection) const
	{
		const integer things = thingSet_.size();
		bool foundThing = false;

		real tClosest = infinity<real>();
		for (integer i = 0;i < things;++i)
		{
			const ThingPtr thing = thingSet_[i];

			ShapeIntersection intersection;
			if (thing->intersect(ray, intersection))
			{
				if (intersection.t < tClosest)
				{
					shapeIntersection = intersection;
					tClosest = intersection.t;
					foundThing = true;
				}
			}
		}
		
		return foundThing;
	}

}
