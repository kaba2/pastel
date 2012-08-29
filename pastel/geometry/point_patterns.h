// Description: Generation and modification of point-sets

#ifndef PASTELGEOMETRY_POINT_PATTERNS_H
#define PASTELGEOMETRY_POINT_PATTERNS_H

#include "pastel/sys/vector.h"

#include <vector>

namespace Pastel
{

	template <typename Real, int N>
	void generateUniformBallPointSet(
		integer points,
		integer dimension,
		std::vector<Vector<Real, N> >& pointSet);

	template <typename Real, int N>
	void generateClusteredPointSet(
		integer points,
		integer dimension,
		integer clusters,
		std::vector<Vector<Real, N> >& pointSet);

	template <typename Real, int N>
	void generateUniformCubePointSet(
		integer points,
		integer dimension,
		std::vector<Vector<Real, N> >& pointSet);

	template <typename Real, int N>
	void generateGaussianPointSet(
		integer points,
		integer dimension,
		std::vector<Vector<Real, N> >& pointSet);

	template <typename Real, int N>
	void scale(
		const Vector<Real, N>& scaling,
		std::vector<Vector<Real, N> >& pointSet);

	template <typename Real, int N>
	void randomlyRotate(
		std::vector<Vector<Real, N> >& pointSet);

	template <typename Real, int N>
	void randomlyReduceDimensionality(
		integer dimensionality,
		std::vector<Vector<Real, N> >& pointSet);

}

#include "pastel/geometry/point_patterns.hpp"

#endif
