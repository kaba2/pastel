#ifndef PASTEL_KDTREE_THING_HPP
#define PASTEL_KDTREE_THING_HPP

#include "pastel/ray/kdtree_thing.h"

namespace Pastel
{
	
	template <typename ThingPtr_Iterator>
	void KdTree_Thing::insert(const ForwardRange<ThingPtr_Iterator>& thingSet)
	{
		kdTree_.insert(thingSet.begin(), thingSet.end());
	}

}

#endif
