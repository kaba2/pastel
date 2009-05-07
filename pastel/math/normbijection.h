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
			return square(norm);
		}

		template <typename Expression>
		Real compute(const VectorExpression<N, Real, Expression>& that) const
		{
			return dot(that);
		}

		template <typename Expression>
		Real compute(const VectorExpression<N, Real, Expression>& that,
			const PASTEL_NO_DEDUCTION(Real)& threshold) const
		{
			const integer dimension = that.size();
			
			Real result = 0;
			for (integer i = 0;i < dimension;++i)
			{
				result += square(that[i]);
				if (result > threshold)
				{
					return infinity<Real>();
				}
			}

			return result;
		}

		Real computeAxis(integer axis, const Real& that) const
		{
			return square(that);
		}

		Real replaceAxis(
			const Real& distance, 
			const Real& currentAxisDistance,
			const Real& newAxisDistance) const
		{
			return (distance - currentAxisDistance) + newAxisDistance;
		}
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
			return norm;
		}

		template <typename Expression>
		Real compute(const VectorExpression<N, Real, Expression>& that) const
		{
			return normInfinity(that);
		}

		template <typename Expression>
		Real compute(const VectorExpression<N, Real, Expression>& that,
			const PASTEL_NO_DEDUCTION(Real)& threshold) const
		{
			const integer dimension = that.size();
			
			Real result = 0;
			for (integer i = 0;i < dimension;++i)
			{
				Real current = mabs(that[i]);
				if (current > result)
				{
					if (result > threshold)
					{
						return infinity<Real>();
					}
					result = current;
				}
			}

			return result;
		}

		Real computeAxis(integer axis, const Real& that) const
		{
			return mabs(that);
		}

		Real replaceAxis(
			const Real& distance, 
			const Real& currentAxisDistance,
			const Real& newAxisDistance) const
		{
			if (distance < newAxisDistance)
			{
				return newAxisDistance;
			}
			
			return distance;
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
			return norm;
		}

		template <typename Expression>
		Real compute(const VectorExpression<N, Real, Expression>& that) const
		{
			return normManhattan(that);
		}

		template <typename Expression>
		Real compute(
			const VectorExpression<N, Real, Expression>& that,
			const PASTEL_NO_DEDUCTION(Real)& threshold) const
		{
			const integer dimension = that.size();
			
			Real result = 0;
			for (integer i = 0;i < dimension;++i)
			{
				result += mabs(that[i]);
				if (result > threshold)
				{
					return infinity<Real>();
				}
			}

			return result;
		}

		Real computeAxis(integer axis, const Real& that) const
		{
			return mabs(that);
		}

		Real replaceAxis(
			const Real& distance, 
			const Real& currentAxisDistance,
			const Real& newAxisDistance) const
		{
			return (distance - currentAxisDistance) + newAxisDistance;
		}
	};

}

#endif
