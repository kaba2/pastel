#ifndef PASTEL_SET_THING_HPP
#define PASTEL_SET_THING_HPP

#include "pastel/ray/set_thing.h"

#include "pastel/geometry/bounding_alignedbox_alignedbox.h"

namespace Pastel
{

	template <typename ThingPtr_Iterator>
	void Set_Thing::insert(
		const ForwardIterator_Range<ThingPtr_Iterator>& thingSet)
	{
		ThingPtr_Iterator iter = thingSet.begin();
		const ThingPtr_Iterator iterEnd = thingSet.end();
		while(iter != iterEnd)
		{
			const ThingPtr thing = *iter;
			thingSet_.push_back(thing);
			extendToCover(thing->bound(), bound_);
			++iter;
		}
	}

}

#endif
