// Description: Norm archetype
// Documentation: norms.txt

#ifndef PASTELMATH_NORM_ARCHETYPE_H
#define PASTELMATH_NORM_ARCHETYPE_H

#include "pastel/math/norm/norm_concept.h"

namespace Pastel
{

	class Norm_Archetype
	{
	public:
		auto operator()() const
		{
			return Distance_Archetype();
		}
	};

	PASTEL_CONCEPT_CHECK(Norm_Archetype, Norm_Concept);

}

#endif
