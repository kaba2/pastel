// Description: Minkowski_NormBijection class
// Detail: A norm bijection of the Minkowski norm (p-norm)
// Documentation: normbijection.txt

#ifndef PASTEL_MINKOWSKI_NORMBIJECTION_H
#define PASTEL_MINKOWSKI_NORMBIJECTION_H

#include "pastel/sys/vector_tools.h"

#include "pastel/geometry/sphere_volume.h"

namespace Pastel
{

	template <typename Real_>
	class Minkowski_NormBijection
	{
	public:
		typedef Real_ Real;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		explicit Minkowski_NormBijection(
			const Real& power)
			: power_(power)
			, invPower_(inverse(power))
		{
			ENSURE_OP(power, >=, 1);
		}

		// No idea how to compute this one!
		/*
		Real lnVolumeUnitSphere(integer dimension) const
		{
			// FIX: TODO
		}
		*/

		Real toNorm(const Real& normBijection) const
		{
			return std::pow(normBijection, invPower_);
		}

		Real toLnNorm(const Real& normBijection) const
		{
			return invPower_ * std::log(normBijection);
		}

		Real toBijection(const Real& norm) const
		{
			PENSURE_OP(norm, >=, 0);
			return std::pow(norm, power_);
		}

		Real scalingFactor(
			const Real& scaling) const
		{
			return std::pow(mabs(scaling), power_);
		}

		Real axis(
			const Real& axisDistance) const
		{
			return std::pow(axisDistance, power_);
		}

		Real signedAxis(
			const Real& axisDistance) const
		{
			return std::pow(mabs(axisDistance), power_);
		}

		Real addAxis(
			const Real& distance, 
			const Real& newAxisDistance) const
		{
			PENSURE_OP(newAxisDistance, >=, 0);

			return distance + 
				newAxisDistance;
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

	private:
		Real power_;
		Real invPower_;
	};

}

#endif
