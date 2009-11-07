#ifndef PASTEL_SET_THING_H
#define PASTEL_SET_THING_H

#include "pastel/ray/composite_thing.h"
#include "pastel/ray/thing.h"

#include "pastel/sys/forwardrange.h"

#include <vector>

namespace Pastel
{

	class PASTELRAY Set_Thing
		: public Composite_Thing
	{
	public:
		virtual ~Set_Thing();

		template <typename ThingPtr_Iterator>
		void insert(
			const ForwardRange<ThingPtr_Iterator>& thingSet);

	private:
		virtual bool intersect_(
			const Ray3& ray,
			LocalGeometry& surface,
			real& tClosest) const;

		std::vector<ThingPtr> thingSet_;
	};

	typedef CountedPtr<Set_Thing> Set_ThingPtr;

}

#include "pastel/ray/set_thing.hpp"

#endif
