#ifndef PASTEL_POINT_PATTERNS_H
#define PASTEL_POINT_PATTERNS_H

#include "pastel/sys/point.h"

#include <vector>

namespace Pastel
{

	template <int N, typename Real>
	void generateUniformBallPointSet(integer points,
						 integer dimension,
						 std::vector<Point<N, Real> >& pointSet);

	template <int N, typename Real>
	void generateClusteredPointSet(integer points,
						 integer dimension,
						 integer clusters,
						 std::vector<Point<N, Real> >& pointSet);

	template <int N, typename Real>
	void generateUniformCubePointSet(integer points,
						 integer dimension,
						 std::vector<Point<N, Real> >& pointSet);

	template <int N, typename Real>
	void generateGaussianPointSet(integer points,
						 integer dimension,
						 std::vector<Point<N, Real> >& pointSet);

}

#include "pastel/geometry/point_patterns.hpp"

#endif
