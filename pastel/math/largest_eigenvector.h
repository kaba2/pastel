// Description: Dominant eigenvector of a point set
// Detail: Computes largest eigenvector of the covariance matrix of points
// Documentation: linear_algebra.txt

#ifndef PASTEL_LARGEST_EIGENVECTOR_H
#define PASTEL_LARGEST_EIGENVECTOR_H

#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"

#include <vector>

namespace Pastel
{

	template <int N, typename Real>
	Vector<Real, N> largestEigenVector(
		const std::vector<Point<Real, N> >& pointSet);

}

#include "pastel/math/largest_eigenvector.hpp"

#endif
