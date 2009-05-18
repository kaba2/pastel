#ifndef PASTELMATH_NORMBIJECTION_H
#define PASTELMATH_NORMBIJECTION_H

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/syscommon.h"

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

	template <int N, typename Real>
	class EuclideanNormBijection
	{
	public:
		Real toNorm(const Real& normBijection) const
		{
			return std::sqrt(normBijection);
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
			const Real& distanceBijection, 
			const Real& newAxisDistance) const
		{
			PENSURE(newAxisDistance >= 0);

			return distanceBijection + 
				newAxisDistance;
		}

		Real replaceAxis(
			const Real& distanceBijection, 
			const Real& oldAxisDistance,
			const Real& newAxisDistance) const
		{
			PENSURE(oldAxisDistance >= 0);
			PENSURE(newAxisDistance >= 0);

			return (distanceBijection - oldAxisDistance) + 
				newAxisDistance;
		}
	};

	template <int N, typename Real>
	class MinkowskiNormBijection
	{
	public:
		explicit MinkowskiNormBijection(const Real& power)
			: power_(power)
			, invPower_(inverse(power))
		{
			PENSURE(power >= 1);
		}

		Real toNorm(const Real& normBijection) const
		{
			return std::pow(normBijection, invPower_);
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
			const Real& distanceBijection, 
			const Real& newAxisDistance) const
		{
			PENSURE(newAxisDistance >= 0);

			return distanceBijection + 
				newAxisDistance;
		}

		Real replaceAxis(
			const Real& distanceBijection, 
			const Real& oldAxisDistance,
			const Real& newAxisDistance) const
		{
			PENSURE(oldAxisDistance >= 0);
			PENSURE(newAxisDistance >= 0);

			return (distanceBijection - oldAxisDistance) + 
				newAxisDistance;
		}

	private:
		Real power_;
		Real invPower_;
	};

	template <int N, typename Real>
	class InfinityNormBijection
	{
	public:
		Real toNorm(const Real& normBijection) const
		{
			return normBijection;
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
			const Real& distanceBijection, 
			const Real& newAxisDistance) const
		{
			PENSURE(newAxisDistance >= 0);

			return std::max(
				distanceBijection,
				newAxisDistance);
		}

		Real replaceAxis(
			const Real& distanceBijection, 
			const Real& oldAxisDistance,
			const Real& newAxisDistance) const
		{
			PENSURE(oldAxisDistance >= 0);
			PENSURE(newAxisDistance >= 0);

			return std::max(
				distanceBijection, 
				newAxisDistance);
		}
	};

	template <int N, typename Real>
	class ManhattanNormBijection
	{
	public:
		Real toNorm(const Real& normBijection) const
		{
			return normBijection;
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
			const Real& distanceBijection, 
			const Real& newAxisDistance) const
		{
			PENSURE(newAxisDistance >= 0);

			return distanceBijection + newAxisDistance;
		}

		Real replaceAxis(
			const Real& distanceBijection, 
			const Real& oldAxisDistance,
			const Real& newAxisDistance) const
		{
			PENSURE(oldAxisDistance >= 0);
			PENSURE(newAxisDistance >= 0);

			return (distanceBijection - oldAxisDistance) + 
				newAxisDistance;
		}
	};

}

#endif
