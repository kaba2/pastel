// Description: Norm-bijection archetype
// Documentation: normbijection.txt

#ifndef PASTELMATH_NORMBIJECTION_ARCHETYPE_H
#define PASTELMATH_NORMBIJECTION_ARCHETYPE_H

#include "pastel/math/normbijection/normbijection_concept.h"

namespace Pastel
{

	class NormBijection_Archetype
	{
	public:
		using Real = real;

		Real lnVolumeUnitSphere(integer dimension) const
		{
			return 0;
		}

		Real toNorm(const Real& normBijection) const
		{
			return 0;
		}

		Real toLnNorm(const Real& normBijection) const
		{
			return 0;
		}

		Real toBijection(const Real& norm) const
		{
			return 0;
		}

		Real scalingFactor(
			const Real& scaling) const
		{
			return 0;
		}

		Real axis(
			const Real& axisDistance) const
		{
			return 0;
		}

		Real signedAxis(
			const Real& axisDistance) const
		{
			return 0;
		}

		Real addAxis(
			const Real& distance, 
			const Real& newAxisDistance) const
		{
			return 0;
		}

		Real replaceAxis(
			const Real& distance, 
			const Real& oldAxisDistance,
			const Real& newAxisDistance) const
		{
			return 0;
		}
	};

	PASTEL_CONCEPT_CHECK(NormBijection_Archetype, NormBijection_Concept);

}

#endif
