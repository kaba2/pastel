// Description: Eigenvectors and eigenvalues of point-sets
// Detail: Eigenstructure of the covariance matrix of a point-set

#ifndef PASTELGEOMETRY_POINTSET_EIGEN_H
#define PASTELGEOMETRY_POINTSET_EIGEN_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/range.h"

#include "pastel/math/matrix/matrix.h"
#include "pastel/math/statistic.h"

namespace Pastel
{

	//! Computes an approximation of the largest eigenvector of a point-set.
	/*!
	Time complexity:
	O(pointSet.size() * dimension)
	*/
	template <
		typename Point_ConstRange, 
		typename Locator>
	Vector<typename Locator::Real, Locator::N> 
	largestEigenVector(
		const Point_ConstRange& pointSet,
		const Locator& locator)
	{
		using Real = typename Locator::Real;
		static constexpr int N = Locator::N;

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

		Vector<Real, N> meanPoint = 
			pointSetMean(pointSet, locator);

		// We choose the initial approximation as
		// the direction of greatest axis-aligned variance.
		// Simply setting it to e_1 = (1, 0, ..., 0)
		// does not always work, probably due to numerical
		// errors.

		Vector<Real, N> axisVariance = 
			pointSetVariance(pointSet, meanPoint, locator);

		integer initialAxis = maxIndex(axisVariance);

		Vector<Real, N> result(
			unitAxis<Real, N>(n, initialAxis));
		Real d = 1;

		Point_ConstIterator iter = pointSet.begin();
		Point_ConstIterator iterEnd = pointSet.end();
		while(iter != iterEnd)
		{
			Real y = dot(result, 

				pointAsVector(location(*iter, locator)) - meanPoint);

			// We take beta = 1.
			
			//d = beta * d + square(y);
			d += square(y);

			result += ((pointAsVector(location(*iter, locator)) - meanPoint) - 
				result * y) * (y / d);

			++iter;
		}

		return result;
	}

	//! Computes an approximation of the eigenstructure of a point-set.
	/*!
	Preconditions:
	eigenvectors > 0

	Time complexity:
	O(pointSet.size() * dimension * eigenvectors)
	
	pointSet:
	A set of vectors.

	eigenvectors:
	The number of dominant eigenvectors that is
	to be approximated.
	
	qOut:
	A matrix containing the approximated eigenvectors 
	as row vectors.

	dOut:
	A vector containing the approximated eigenvalues
	corresponding to the approximated eigenvectors
	in qOut.
	*/
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

		dreal beta = 1;
		Vector<Real> meanPoint = 
			pointSetMean(pointSet, locator);

		qOut = identityMatrix<Real>(eigenvectors, n);
		dOut.setSize(eigenvectors);
		dOut = Vector<Real>(ofDimension(eigenvectors), 1);

		Vector<Real> x(ofDimension(n));

		auto iter = pointSet.begin();
		auto iterEnd = pointSet.end();
		while(iter != iterEnd)
		{

			x = pointAsVector(location(*iter, locator)) - meanPoint;

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
