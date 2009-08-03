#ifndef PASTEL_LARGEST_EIGENVECTOR_HPP
#define PASTEL_LARGEST_EIGENVECTOR_HPP

#include "pastel/math/largest_eigenvector.h"
#include "pastel/math/statistics.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	TemporaryVector<Real, N> largestEigenVector(
		const std::vector<Point<Real, N> >& pointSet)
	{
		// This is the PASTd algorithm from
		// "Projection Approximation Subspace Tracking",
		// Bin Yang, IEEE Transactions on Signal Processing,
		// Vol 43., No. 1, January 1995.

		ENSURE(!pointSet.empty());

		const integer points = pointSet.size();
		const integer dimension = pointSet.front().dimension();

		const Point<Real, N> meanPoint = mean(pointSet);

		// We choose the initial approximation as
		// the direction of greatest axis aligned variance.
		// Simply setting it to e_1 = (1, 0, ..., 0)
		// does not always work, probably due to numerical
		// errors.

		const Vector<Real, N> axisVariance = axisAlignedVariance(
			pointSet, meanPoint);

		const integer initialAxis = maxIndex(axisVariance);

		Vector<Real, N> result(unitAxis<Real, N>(dimension, initialAxis));
		Real d = 1;

		for (integer i = 0;i < points;++i)
		{
			const Real y = dot(result, pointSet[i] - meanPoint);

			// We take beta = 1.
			
			//d = beta * d + square(y);
			d += square(y);

			result += ((pointSet[i] - meanPoint) - result * y) * (y / d);
		}

		return result.asTemporary();
	}

}

#endif
