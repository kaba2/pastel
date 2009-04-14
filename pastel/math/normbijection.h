#ifndef PASTELMATH_NORMBIJECTION_H
#define PASTELMATH_NORMBIJECTION_H

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/syscommon.h"

#include <cmath>

namespace Pastel
{

	// Let |x| : R^n -> R+ be a norm and
	// f an order-preserving bijection R+ -> R+.
	// Then <x> : R^n -> R+ : <x> = f(|x|).
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
		Real operator()(const Vector<N, Real>& that) const
		{
			return dot(that);
		}

		Real operator()(integer axis, const Real& that) const
		{
			return square(that);
		}
	};

	template <int N, typename Real>
	class InfinityNormBijection
	{
	public:
		Real operator()(const Vector<N, Real>& that) const
		{
			return normInfinity(that);
		}

		Real operator()(integer axis, const Real& that) const
		{
			return std::abs(that);
		}
	};

	template <int N, typename Real>
	class ManhattanNormBijection
	{
	public:
		Real operator()(const Vector<N, Real>& that) const
		{
			return normManhattan(that);
		}

		Real operator()(integer axis, const Real& that) const
		{
			return std::abs(that);
		}
	};

}

#endif
