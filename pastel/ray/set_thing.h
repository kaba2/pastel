#ifndef PASTEL_SET_THING_H
#define PASTEL_SET_THING_H

#include "pastel/ray/composite_thing.h"
#include "pastel/ray/thing.h"

#include "pastel/sys/range.h"

#include <vector>

namespace Pastel
{

	class PASTELRAY Set_Thing
		: public Composite_Thing
	{
	public:
		virtual ~Set_Thing();

		template <typename ThingPtr_Range>
		void insert(
			const ThingPtr_Range& thingSet);

		virtual AlignedBox3 bound() const;

	private:
		virtual bool intersect_(
			const Ray3& ray,
			ShapeIntersection& shapeIntersection) const;

		std::vector<ThingPtr> thingSet_;
		AlignedBox3 bound_;
	};

	typedef CountedPtr<Set_Thing> Set_ThingPtr;

}

#include "pastel/ray/set_thing.hpp"

#endif
