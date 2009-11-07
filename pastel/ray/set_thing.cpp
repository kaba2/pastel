#include "pastel/ray/set_thing.h"
#include "pastel/ray/shape_thing.h"

namespace Pastel
{

	Set_Thing::~Set_Thing()
	{
	}

	// Private

	bool Set_Thing::intersect_(
		const Ray3& ray,
		LocalGeometry& surface,
		real& tClosest) const
	{
		const integer things = thingSet_.size();
		bool foundThing = false;

		tClosest = infinity<real>();
		for (integer i = 0;i < things;++i)
		{
			const ThingPtr thing = thingSet_[i];

			real tThing = 0;
			if (thing->intersect(ray, surface, tThing))
			{
				if (tThing < tClosest)
				{
					tClosest = tThing;
					foundThing = true;
				}
			}
		}
		
		return foundThing;
	}

}
