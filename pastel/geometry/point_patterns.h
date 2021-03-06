// Description: Generation and modification of point-sets
// Documentation: undocumented.txt

#ifndef PASTELGEOMETRY_POINT_PATTERNS_H
#define PASTELGEOMETRY_POINT_PATTERNS_H

#include "pastel/sys/vector.h"
#include "pastel/sys/random.h"

#include "pastel/math/sampling/uniform_sampling.h"
#include "pastel/math/matrix/orthonormal.h"

#include <vector>

namespace Pastel
{

	template <typename Real, int N>
	void generateUniformBallPointSet(
		integer points,
		integer dimension,
		std::vector<Vector<Real, N> >& pointSet)
	{
		ENSURE_OP(points, >, 0);
		ENSURE_OP(dimension, >, 0);
		ENSURE2(N == Dynamic || N == dimension, N, dimension);

		std::vector<Vector<Real, N> > result;
		result.reserve(points);

		for (integer i = 0;i < points;++i)
		{
			result.push_back(randomVectorBall<Real, N>(dimension));
		}

		result.swap(pointSet);
	}

	template <typename Real, int N>
	void generateClusteredPointSet(
		integer points,
		integer dimension,
		integer clusters,
		std::vector<Vector<Real, N> >& pointSet)
	{
		ENSURE_OP(points, >, 0);
		ENSURE_OP(dimension, >, 0);
		ENSURE_OP(clusters, >, 0);
		ENSURE2(N == Dynamic || N == dimension, N, dimension);

		std::vector<Vector<Real, N> > result;
		result.reserve(points);

		Real realPointsCreated = 0;
		Real realClusterPoints = (Real)points / clusters;

		integer pointsCreated = 0;

		for (integer j = 0;j < clusters;++j)
		{
			integer clusterPoints = 
				(realPointsCreated + realClusterPoints) - pointsCreated;
			if (j == clusters - 1)
			{
				clusterPoints = points - pointsCreated;
			}

			Vector<Real, N> clusterCenter(
				randomVectorCube<Real, N>(dimension));

			const Real clusterRadius = random<Real>() * 0.1;

			for (integer i = 0;i < clusterPoints;++i)
			{
				result.push_back(
					clusterCenter + 
					randomGaussianVector<Real, N>(dimension) * clusterRadius);
			}

			realPointsCreated += realClusterPoints;
			pointsCreated += clusterPoints;
		}

		result.swap(pointSet);
	}

	template <typename Real, int N>
	void generateUniformCubePointSet(
		integer points,
		integer dimension,
		std::vector<Vector<Real, N> >& pointSet)
	{
		ENSURE_OP(points, >, 0);
		ENSURE_OP(dimension, >, 0);
		ENSURE2(N == Dynamic || N == dimension, N, dimension);

		std::vector<Vector<Real, N> > result;
		result.reserve(points);

		for (integer i = 0;i < points;++i)
		{
			result.push_back(
				randomVectorCube<Real, N>(dimension));
		}

		result.swap(pointSet);
	}

	template <typename Real, int N>
	void generateGaussianPointSet(
		integer points,
		integer dimension,
		std::vector<Vector<Real, N> >& pointSet)
	{
		ENSURE_OP(points, >, 0);
		ENSURE_OP(dimension, >, 0);
		ENSURE2(N == Dynamic || N == dimension, N, dimension);

		std::vector<Vector<Real, N> > result;
		result.reserve(points);

		for (integer i = 0;i < points;++i)
		{
			result.push_back(
				randomGaussianVector<Real, N>(dimension));
		}

		result.swap(pointSet);
	}

	template <typename Real, int N>
	void scale(
		const Vector<Real, N>& scaling,
		std::vector<Vector<Real, N> >& pointSet)
	{
		if (pointSet.empty())
		{
			return;
		}

		integer points = pointSet.size();
		for (integer i = 0;i < points;++i)
		{
			pointSet[i] *= scaling;
		}
	}

	template <typename Real, int N>
	void randomlyRotate(
		std::vector<Vector<Real, N> >& pointSet)
	{
		if (pointSet.empty())
		{
			return;
		}

		integer points = pointSet.size();
		integer dimension = pointSet.front().n();

		Matrix<Real, N, N> rotation = randomRotation(dimension);
		for (integer i = 0;i < points;++i)
		{
			pointSet[i] = pointSet[i] * rotation;
		}
	}

	template <typename Real, int N>
	void randomlyReduceDimensionality(
		integer dimensionality,
		std::vector<Vector<Real, N> >& pointSet)
	{
		ENSURE_OP(dimensionality, >, 0);

		integer dimension = pointSet.front().n();

		ENSURE_OP(dimensionality, <=, dimension);

		if (pointSet.empty() || dimensionality == dimension)
		{
			return;
		}
	
		std::vector<integer> indexSet;
		for (integer i = 0;i < dimension;++i)
		{
			indexSet.push_back(i);
		}

		std::random_shuffle(
			indexSet.begin(), indexSet.end());

		integer dimensionsToRemove = dimension - dimensionality;

		integer points = pointSet.size();
		for (integer i = 0;i < points;++i)
		{
			for (integer j = 0;j < dimensionsToRemove;++j)
			{
				pointSet[i][indexSet[j]] = 0;
			}
		}
	}

}

#endif
