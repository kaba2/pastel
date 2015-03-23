// Description: Manhattan_NormBijection class
// Detail: A norm bijection of the Manhattan norm
// Documentation: normbijection.txt

#ifndef PASTELMATH_MANHATTAN_NORMBIJECTION_H
#define PASTELMATH_MANHATTAN_NORMBIJECTION_H

#include "pastel/sys/vector/vector_tools.h"

#include "pastel/geometry/volume/sphere_volume.h"

namespace Pastel
{

	template <typename Real_>
	class Manhattan_NormBijection
	{
	public:
		using Real = Real_;

		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Real lnVolumeUnitSphere(integer dimension) const
		{
			return Pastel::lnVolumeUnitSphereManhattan<Real>(dimension);
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
			return abs(scaling);
		}

		Real axis(
			const Real& axisDistance) const
		{
			return axisDistance;
		}

		Real signedAxis(
			const Real& axisDistance) const
		{
			return abs(axisDistance);
		}

		Real addAxis(
			const Real& distance, 
			const Real& newAxisDistance) const
		{
			PENSURE_OP(newAxisDistance, >=, 0);

			return distance + newAxisDistance;
		}

		Real replaceAxis(
			const Real& distance, 
			const Real& oldAxisDistance,
			const Real& newAxisDistance) const
		{
			PENSURE_OP(oldAxisDistance, >=, 0);
			PENSURE_OP(newAxisDistance, >=, 0);

			return (distance - oldAxisDistance) + 
				newAxisDistance;
		}
	};

}

#endif
