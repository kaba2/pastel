#ifndef PASTEL_SET_THING_HPP
#define PASTEL_SET_THING_HPP

#include "pastel/ray/set_thing.h"

namespace Pastel
{

	template <typename ThingPtr_Iterator>
	void Set_Thing::insert(
		const ForwardRange<ThingPtr_Iterator>& thingSet)
	{
		thingSet_.insert(
			thingSet_.end(),
			thingSet.begin(), thingSet.end());
	}

}

#endif
