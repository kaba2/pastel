#ifndef PASTEL_NORMBIJECTION_H
#define PASTEL_NORMBIJECTION_H

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/syscommon.h"

#include "pastel/geometry/sphere_volume.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	class EuclideanNormBijection
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Real lnVolumeUnitSphere(integer dimension) const
		{
			return Pastel::lnVolumeUnitSphereEuclidean<Real>(dimension);
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
			PENSURE_OP(norm, >=, 0);
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
	};

	template <typename Real>
	class MinkowskiNormBijection
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		explicit MinkowskiNormBijection(
			const Real& power)
			: power_(power)
			, invPower_(inverse(power))
		{
			ENSURE(N != Dynamic);
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

	template <typename Real>
	class InfinityNormBijection
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

	template <typename Real>
	class ManhattanNormBijection
	{
	public:
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
