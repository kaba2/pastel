#ifndef PASTELGEOMETRY_POINTSET_EIGEN_HPP
#define PASTELGEOMETRY_POINTSET_EIGEN_HPP

#include "pastel/geometry/pointset_eigen.h"

#include "pastel/math/statistics.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <
		typename Point_ConstRange, 
		typename Locator>
	Vector<typename Locator::Real, Locator::N> 
	largestEigenVector(
		const Point_ConstRange& pointSet,
		const Locator& locator)
	{
		using Real = typename Locator::Real;
		static PASTEL_CONSTEXPR int N = Locator::N;

		// This is the PASTd algorithm from
		// "Projection Approximation Subspace Tracking",
		// Bin Yang, IEEE Transactions on Signal Processing,
		// Vol 43., No. 1, January 1995.

		integer n = locator.n();
		ENSURE_OP(n, !=, Dynamic);

		if (pointSet.empty())
		{
			return Vector<Real, N>(ofDimension(n), 0);
		}

		const Vector<Real, N> meanPoint = 
			pointSetMean(pointSet, locator);

		// We choose the initial approximation as
		// the direction of greatest axis-aligned variance.
		// Simply setting it to e_1 = (1, 0, ..., 0)
		// does not always work, probably due to numerical
		// errors.

		const Vector<Real, N> axisVariance = 
			pointSetVariance(pointSet, meanPoint, locator);

		const integer initialAxis = maxIndex(axisVariance);

		Vector<Real, N> result(
			unitAxis<Real, N>(n, initialAxis));
		Real d = 1;

		Point_ConstIterator iter = pointSet.begin();
		const Point_ConstIterator iterEnd = pointSet.end();
		while(iter != iterEnd)
		{
			const Real y = dot(result, 

				pointAsVector(*iter, locator) - meanPoint);

			// We take beta = 1.
			
			//d = beta * d + square(y);
			d += square(y);

			result += ((pointAsVector(*iter, locator) - meanPoint) - 
				result * y) * (y / d);

			++iter;
		}

		return result;
	}

	template <typename Point_ConstRange, typename Locator>
	void approximateEigenstructure(
		const Point_ConstRange& pointSet,
		const Locator& locator,
		integer eigenvectors,
		Matrix<typename Locator::Real>& qOut,
		Vector<typename Locator::Real>& dOut)
	{
		// This is the PASTd algorithm from
		// "Projection Approximation Subspace Tracking",
		// Bin Yang, IEEE Transactions on Signal Processing,
		// Vol 43., No. 1, January 1995.

		using Real = typename Locator::Real;

		ENSURE(!pointSet.empty());
		ENSURE_OP(eigenvectors, >, 0);

		integer n = locator.n();
		ENSURE_OP(n, !=, Dynamic);

		const real beta = 1;
		const Vector<Real> meanPoint = 
			pointSetMean(pointSet, locator);

		qOut = identityMatrix<Real>(eigenvectors, n);
		dOut.setSize(eigenvectors);
		dOut = Vector<Real>(ofDimension(eigenvectors), 1);

		Vector<Real> x(ofDimension(n));

		auto iter = pointSet.begin();
		auto iterEnd = pointSet.end();
		while(iter != iterEnd)
		{

			x = pointAsVector(*iter, locator) - meanPoint;

			for (integer j = 0;j < eigenvectors;++j)
			{
				Real& d = dOut[j];

				Real y = dot(qOut[j], x);


				d = beta * d + square(y);

				qOut[j] += (x - qOut[j] * y) * (y / d);

				x -= qOut[j] * y;
			}

			++iter;
		}
	}

}

#endif
