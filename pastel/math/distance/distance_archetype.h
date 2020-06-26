// Description: Distance archetype
// Documentation: distances.txt

#ifndef PASTELMATH_DISTANCE_ARCHETYPE_H
#define PASTELMATH_DISTANCE_ARCHETYPE_H

#include "pastel/math/distance/distance_concept.h"
#include "pastel/math/distance/distance_base.h"

namespace Pastel
{

	class Distance_Archetype
	: DistanceBase<Distance_Archetype, dreal>
	{
	public:
		using Real_ = dreal;

		Distance_Archetype() = default;
		explicit Distance_Archetype(Real_) {}

		Distance_Archetype(const Distance_Archetype&) = default;
		Distance_Archetype(Distance_Archetype&&) = default;
		Distance_Archetype& operator=(const Distance_Archetype&) = default;

		explicit operator Real() const {
			return 0;
		}

		Real operator~() const {
			return 0;
		}

		decltype(auto) replace(integer axis, const Real& from, const Real& to) {
			return *this;
		}

		decltype(auto) operator*=(const Real& amount) {
			return *this;
		}
	};

	PASTEL_CONCEPT_CHECK(Distance_Archetype, Distance_Concept);

}

#endif
