#ifndef PASTEL_POINTSET_EIGEN_HPP
#define PASTEL_POINTSET_EIGEN_HPP

#include "pastel/geometry/pointset_eigen.h"

#include "pastel/math/statistics.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <
		typename Point_ConstIterator, 
		typename PointPolicy>
	Vector<typename PointPolicy::Coordinate, PointPolicy::N> 
	largestEigenVector(
		const ForwardRange<Point_ConstIterator>& pointSet,
		const PointPolicy& pointPolicy)
	{
		typedef typename PointPolicy::Coordinate Real;
		enum {N = PointPolicy::N};

		// This is the PASTd algorithm from
		// "Projection Approximation Subspace Tracking",
		// Bin Yang, IEEE Transactions on Signal Processing,
		// Vol 43., No. 1, January 1995.

		const integer n = pointPolicy.dimension();
		ENSURE_OP(n, !=, Dynamic);

		if (pointSet.empty())
		{
			return Vector<Real, N>(ofDimension(n), 0);
		}

		const Vector<Real, N> meanPoint = 
			pointSetMean(pointSet, pointPolicy);

		// We choose the initial approximation as
		// the direction of greatest axis-aligned variance.
		// Simply setting it to e_1 = (1, 0, ..., 0)
		// does not always work, probably due to numerical
		// errors.

		const Vector<Real, N> axisVariance = 
			pointSetVariance(pointSet, meanPoint, pointPolicy);

		const integer initialAxis = maxIndex(axisVariance);

		Vector<Real, N> result(
			unitAxis<Real, N>(n, initialAxis));
		Real d = 1;

		Point_ConstIterator iter = pointSet.begin();
		const Point_ConstIterator iterEnd = pointSet.end();
		while(iter != iterEnd)
		{
			const Real y = dot(result, 
				pointPolicy(*iter) - meanPoint);

			// We take beta = 1.
			
			//d = beta * d + square(y);
			d += square(y);

			result += ((pointPolicy(*iter) - meanPoint) - 
				result * y) * (y / d);

			++iter;
		}

		return result;
	}

	template <typename Point_ConstIterator, typename PointPolicy>
	void approximateEigenstructure(
		const ForwardRange<Point_ConstIterator>& pointSet,
		const PointPolicy& pointPolicy,
		integer eigenvectors,
		Matrix<typename PointPolicy::Coordinate>& qOut,
		Vector<typename PointPolicy::Coordinate>& dOut)
	{
		// This is the PASTd algorithm from
		// "Projection Approximation Subspace Tracking",
		// Bin Yang, IEEE Transactions on Signal Processing,
		// Vol 43., No. 1, January 1995.

		typedef typename PointPolicy::Coordinate Real;

		ENSURE(!pointSet.empty());
		ENSURE_OP(eigenvectors, >, 0);

		const integer n = pointPolicy.dimension();
		ENSURE_OP(n, !=, Dynamic);

		const real beta = 1;
		const Vector<Real> meanPoint = 
			pointSetMean(pointSet, pointPolicy);

		qOut = identityMatrix<Real, Dynamic, Dynamic>(eigenvectors, n);
		dOut.setSize(eigenvectors);
		dOut = Vector<Real>(ofDimension(eigenvectors), 1);

		Vector<Real> x(ofDimension(n));

		Point_ConstIterator iter = pointSet.begin();
		const Point_ConstIterator iterEnd = pointSet.end();
		while(iter != iterEnd)
		{
			x = pointPolicy(*iter) - meanPoint;

			for (integer j = 0;j < eigenvectors;++j)
			{
				Real& d = dOut[j];

				const Real y = dot(qOut[j], x);

				d = beta * d + square(y);

				qOut[j] += (x - qOut[j] * y) * (y / d);

				x -= qOut[j] * y;
			}

			++iter;
		}
	}

}

#endif
