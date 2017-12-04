// Description: Norm archetype
// Documentation: norms.txt

#ifndef PASTELMATH_NORM_ARCHETYPE_H
#define PASTELMATH_NORM_ARCHETYPE_H

#include "pastel/math/distance/distance_concept.h"
#include "pastel/math/distance/distance_base.h"

namespace Pastel
{

	class Distance_Archetype
	: NormBase<Distance_Archetype, real>
	{
	public:
		using Real = real;

		explicit operator Real() const {
			return 0;
		}

		Real internal() const {
			return 0;
		}

		auto replace(integer axis, const Real& from, const Real& to) {
			return *this;
		}

		auto operator*=(const Real& amount) {
			return *this;
		}
	};

	PASTEL_CONCEPT_CHECK(Distance_Archetype, Distance_Concept);

}

#endif
