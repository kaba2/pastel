// Description: Infinity_NormBijection class
// Detail: A norm bijection of the infinity norm
// Documentation: normbijection.txt

#ifndef PASTEL_INFINITY_NORMBIJECTION_H
#define PASTEL_INFINITY_NORMBIJECTION_H

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/syscommon.h"

#include "pastel/geometry/sphere_volume.h"

namespace Pastel
{

	template <typename Real>
	class Infinity_NormBijection
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Real lnVolumeUnitSphere(integer dimension) const
		{
			return lnVolumeUnitSphereInfinity<Real>(dimension);
		}

		Real toNorm(const Real& normBijection) const
		{
			return normBijection;
		}

		Real toLnNorm(const Real& normBijection) const
		{
			return std::log(normBijection);
		}

		Real toBijection(const Real& norm) const
		{
			PENSURE_OP(norm, >=, 0);
			return norm;
		}

		Real scalingFactor(
			const Real& scaling) const
		{
			return mabs(scaling);
		}

		Real axis(
			const Real& axisDistance) const
		{
			return axisDistance;
		}

		Real signedAxis(
			const Real& axisDistance) const
		{
			return mabs(axisDistance);
		}

		Real addAxis(
			const Real& distance, 
			const Real& newAxisDistance) const
		{
			PENSURE_OP(newAxisDistance, >=, 0);

			return std::max(
				distance,
				newAxisDistance);
		}

		Real replaceAxis(
			const Real& distance, 
			const Real& oldAxisDistance,
			const Real& newAxisDistance) const
		{
			PENSURE_OP(oldAxisDistance, >=, 0);
			PENSURE_OP(newAxisDistance, >=, 0);

			return std::max(
				distance, 
				newAxisDistance);
		}
	};

}

#endif
