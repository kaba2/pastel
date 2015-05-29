// Description: Euclidean_NormBijection class
// Detail: A norm bijection of the Euclidean norm
// Documentation: normbijection.txt

#ifndef PASTELMATH_EUCLIDEAN_NORMBIJECTION_H
#define PASTELMATH_EUCLIDEAN_NORMBIJECTION_H

#include "pastel/sys/vector/vector_tools.h"
#include "pastel/math/normbijection/normbijection_concept.h"
#include "pastel/geometry/volume/sphere_volume.h"

namespace Pastel
{

	template <typename Real_>
	class Euclidean_NormBijection
	{
	public:
		using Real = Real_;

		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Real lnVolumeUnitSphere(integer dimension) const
		{
			return Pastel::lnVolumeUnitSphere<Real>(dimension);
		}

		Real toNorm(const Real& normBijection) const
		{
			return std::sqrt(normBijection);
		}

		Real toLnNorm(const Real& normBijection) const
		{
			return 0.5 * std::log(normBijection);
		}

		Real toBijection(const Real& norm) const
		{
			PENSURE(norm >= 0);
			return square(norm);
		}

		Real scalingFactor(
			const Real& scaling) const
		{
			return square(scaling);
		}

		Real axis(
			const Real& axisDistance) const
		{
			return square(axisDistance);
		}

		Real signedAxis(
			const Real& axisDistance) const
		{
			return square(axisDistance);
		}

		Real addAxis(
			const Real& distance, 
			const Real& newAxisDistance) const
		{
			PENSURE(newAxisDistance >= 0);

			return distance + 
				newAxisDistance;
		}

		Real replaceAxis(
			const Real& distance, 
			const Real& oldAxisDistance,
			const Real& newAxisDistance) const
		{
			PENSURE(oldAxisDistance >= 0);
			PENSURE(newAxisDistance >= 0);

			return (distance - oldAxisDistance) + 
				newAxisDistance;
		}
	};

}

#endif
