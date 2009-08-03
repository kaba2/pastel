// Description: Tools to generate and modify point sets
// Documentation: pastelgeometry.txt

#ifndef PASTEL_POINT_PATTERNS_H
#define PASTEL_POINT_PATTERNS_H

#include "pastel/sys/point.h"

#include <vector>

namespace Pastel
{

	template <int N, typename Real>
	void generateUniformBallPointSet(
		integer points,
		integer dimension,
		std::vector<Point<Real, N> >& pointSet);

	template <int N, typename Real>
	void generateClusteredPointSet(
		integer points,
		integer dimension,
		integer clusters,
		std::vector<Point<Real, N> >& pointSet);

	template <int N, typename Real>
	void generateUniformCubePointSet(
		integer points,
		integer dimension,
		std::vector<Point<Real, N> >& pointSet);

	template <int N, typename Real>
	void generateGaussianPointSet(
		integer points,
		integer dimension,
		std::vector<Point<Real, N> >& pointSet);

	template <int N, typename Real>
	void scale(
		const Vector<Real, N>& scaling,
		std::vector<Point<Real, N> >& pointSet);

	template <int N, typename Real>
	void randomlyRotate(
		std::vector<Point<Real, N> >& pointSet);

	template <int N, typename Real>
	void randomlyReduceDimensionality(
		integer dimensionality,
		std::vector<Point<Real, N> >& pointSet);

}

#include "pastel/geometry/point_patterns.hpp"

#endif
