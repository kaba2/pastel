// Description: Minkowski_NormBijection class
// Detail: A norm bijection of the Minkowski norm (p-norm)
// Documentation: normbijection.txt

#ifndef PASTELMATH_MINKOWSKI_NORMBIJECTION_H
#define PASTELMATH_MINKOWSKI_NORMBIJECTION_H

#include "pastel/sys/vector/vector_tools.h"
#include "pastel/sys/math/gamma.h"
#include "pastel/sys/math/constants.h"
#include "pastel/math/normbijection/normbijection_concept.h"

#include "pastel/geometry/volume/sphere_volume.h"

namespace Pastel
{

	template <typename Real_>
	class Minkowski_NormBijection
	{
	public:
		using Real = Real_;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		explicit Minkowski_NormBijection(
			const Real& power)
			: power_(power)
			, invPower_(inverse(power))
		{
			// The Minkowski norm is a norm only for p >= 1.
			ENSURE_OP(power, >=, 1);
		}

		const Real& power() const
		{
			return power_;
		}

		const Real& inversePower() const
		{
			return invPower_;
		}

		Real lnVolumeUnitSphere(integer dimension) const
		{
			// From Wikipedia "Volume of an n-ball"
			return 
				lnGamma<Real>(inversePower() + 1) - 
				lnGamma<Real>(dimension * inversePower() + 1) +
				dimension * constantLn2<Real>();
		}

		Real toNorm(const Real& normBijection) const
		{
			return std::pow(normBijection, inversePower());
		}

		Real toLnNorm(const Real& normBijection) const
		{
			return inversePower() * std::log(normBijection);
		}

		Real toBijection(const Real& norm) const
		{
			PENSURE_OP(norm, >=, 0);
			return std::pow(norm, power());
		}

		Real scalingFactor(
			const Real& scaling) const
		{
			return std::pow(abs(scaling), power());
		}

		Real axis(
			const Real& axisDistance) const
		{
			return std::pow(axisDistance, power());
		}

		Real signedAxis(
			const Real& axisDistance) const
		{
			return std::pow(abs(axisDistance), power());
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
