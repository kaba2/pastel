#ifndef PASTEL_LARGEST_EIGENVECTOR_H
#define PASTEL_LARGEST_EIGENVECTOR_H

#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"

#include <vector>

namespace Pastel
{

	template <int N, typename Real>
	TemporaryVector<N, Real> largestEigenVector(
		const std::vector<Point<N, Real> >& pointSet);

}

#include "pastel/math/largest_eigenvector.hpp"

#endif
