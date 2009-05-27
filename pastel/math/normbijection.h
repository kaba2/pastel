#ifndef PASTEL_NORMBIJECTION_H
#define PASTEL_NORMBIJECTION_H

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/syscommon.h"

#include "pastel/geometry/sphere_volume.h"

#include <cmath>

namespace Pastel
{

	// Let |x| : R^n -> R+ be a norm and
	// f an order-preserving bijection R+ -> R+.
	// Then <x> : R^n -> R+ : <x> = f(|x|)
	// is called a norm bijection.
	// Example:
	// |(x, y)| = sqrt(x^2 + y^2)
	// f(r) = r^2
	// <(x, y)> = x^2 + y^2
	// It is usually enough to use
	// norm bijections rather than norms
	// in algorithms.
	// This can be used to enhance performance.

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
			return Pastel::lnVolumeUnitSphereEuclidean<Dynamic, Real>(dimension);
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
			ENSURE(power >= 1);
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
			PENSURE(norm >= 0);
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
			return lnVolumeUnitSphereInfinity<Dynamic, Real>(dimension);
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
			PENSURE(norm >= 0);
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
			PENSURE(newAxisDistance >= 0);

			return std::max(
				distance,
				newAxisDistance);
		}

		Real replaceAxis(
			const Real& distance, 
			const Real& oldAxisDistance,
			const Real& newAxisDistance) const
		{
			PENSURE(oldAxisDistance >= 0);
			PENSURE(newAxisDistance >= 0);

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
			return Pastel::lnVolumeUnitSphereManhattan<Dynamic, Real>(dimension);
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
			PENSURE(norm >= 0);
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
			PENSURE(newAxisDistance >= 0);

			return distance + newAxisDistance;
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
