#ifndef PASTEL_POINTSET_EIGEN_HPP
#define PASTEL_POINTSET_EIGEN_HPP

#include "pastel/geometry/pointset_eigen.h"

#include "pastel/math/statistics.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Vector<Real, N> largestEigenVector(
		const std::vector<Vector<Real, N> >& pointSet)
	{
		// This is the PASTd algorithm from
		// "Projection Approximation Subspace Tracking",
		// Bin Yang, IEEE Transactions on Signal Processing,
		// Vol 43., No. 1, January 1995.

		ENSURE(!pointSet.empty());

		const integer points = pointSet.size();
		const integer dimension = pointSet.front().dimension();

		const Vector<Real, N> meanPoint = mean(pointSet);

		// We choose the initial approximation as
		// the direction of greatest axis-aligned variance.
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

		return result;
	}

	template <typename Real, int N>
	void approximateEigenstructure(
		const std::vector<Vector<Real, N> >& pointSet,
		integer eigenvectors,
		Matrix<Real>& qOut,
		Vector<Real>& dOut)
	{
		// This is the PASTd algorithm from
		// "Projection Approximation Subspace Tracking",
		// Bin Yang, IEEE Transactions on Signal Processing,
		// Vol 43., No. 1, January 1995.

		ENSURE(!pointSet.empty());
		ENSURE_OP(eigenvectors, >, 0);

		const integer points = pointSet.size();
		const integer dimension = pointSet.front().dimension();
		const real beta = 1;

		const Vector<Real, N> meanPoint = mean(pointSet);

		qOut = identityMatrix<Real, Dynamic, Dynamic>(eigenvectors, dimension);
		dOut.setSize(eigenvectors);
		dOut = Vector<Real, Dynamic>(ofDimension(eigenvectors), 1);

		for (integer i = 0;i < points;++i)
		{
			Vector<Real> x = pointSet[i] - meanPoint;
			for (integer j = 0;j < eigenvectors;++j)
			{
				Real& d = dOut[j];

				const Real y = dot(qOut[j], x);

				d = beta * d + square(y);

				qOut[j] += (x - qOut[j] * y) * (y / d);

				x -= qOut[j] * y;
			}
		}
	}

}

#endif
