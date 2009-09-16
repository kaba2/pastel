// Description: Generation and modification of point-sets

#ifndef PASTEL_POINT_PATTERNS_H
#define PASTEL_POINT_PATTERNS_H

#include "pastel/sys/vector.h"

#include <vector>

namespace Pastel
{

	template <int N, typename Real>
	void generateUniformBallPointSet(
		integer points,
		integer dimension,
		std::vector<Vector<Real, N> >& pointSet);

	template <int N, typename Real>
	void generateClusteredPointSet(
		integer points,
		integer dimension,
		integer clusters,
		std::vector<Vector<Real, N> >& pointSet);

	template <int N, typename Real>
	void generateUniformCubePointSet(
		integer points,
		integer dimension,
		std::vector<Vector<Real, N> >& pointSet);

	template <int N, typename Real>
	void generateGaussianPointSet(
		integer points,
		integer dimension,
		std::vector<Vector<Real, N> >& pointSet);

	template <int N, typename Real>
	void scale(
		const Vector<Real, N>& scaling,
		std::vector<Vector<Real, N> >& pointSet);

	template <int N, typename Real>
	void randomlyRotate(
		std::vector<Vector<Real, N> >& pointSet);

	template <int N, typename Real>
	void randomlyReduceDimensionality(
		integer dimensionality,
		std::vector<Vector<Real, N> >& pointSet);

}

#include "pastel/geometry/point_patterns.hpp"

#endif
