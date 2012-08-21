#ifndef PASTEL_KDTREE_THING_HPP
#define PASTEL_KDTREE_THING_HPP

#include "pastel/ray/kdtree_thing.h"

namespace Pastel
{
	
	template <typename ThingPtr_Range>
	void KdTree_Thing::insert(const ThingPtr_Range& thingSet)
	{
		kdTree_.insert(thingSet.begin(), thingSet.end());
	}

}

#endif
