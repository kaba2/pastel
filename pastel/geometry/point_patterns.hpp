#ifndef PASTEL_POINT_PATTERNS_HPP
#define PASTEL_POINT_PATTERNS_HPP

#include "pastel/geometry/point_patterns.h"

#include "pastel/sys/random.h"

#include "pastel/math/uniformsampling.h"

namespace Pastel
{

	template <int N, typename Real>
	void generateUniformBallPointSet(integer points,
						 integer dimension,
						 std::vector<Point<N, Real> >& pointSet)
	{
		ENSURE1(points > 0, points);
		ENSURE1(dimension > 0, dimension);
		ENSURE2(N == Dynamic || N == dimension, N, dimension);

		std::vector<Point<N, Real> > result;
		result.reserve(points);

		for (integer i = 0;i < points;++i)
		{
			result.push_back(
				asPoint(randomVectorBall<N, Real>(dimension)));
		}

		result.swap(pointSet);
	}

	template <int N, typename Real>
	void generateClusteredPointSet(integer points,
						 integer dimension,
						 integer clusters,
						 std::vector<Point<N, Real> >& pointSet)
	{
		ENSURE1(points > 0, points);
		ENSURE1(dimension > 0, dimension);
		ENSURE1(clusters > 0, clusters);
		ENSURE2(N == Dynamic || N == dimension, N, dimension);

		std::vector<Point<N, Real> > result;
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

			const Point<N, Real> clusterCenter(
				randomVectorCube<N, Real>(dimension));
			const Real clusterRadius = random<Real>() * 0.1;

			for (integer i = 0;i < clusterPoints;++i)
			{
				result.push_back(
					clusterCenter + 
					randomGaussianVector<N, Real>(dimension) * clusterRadius);
			}

			realPointsCreated += realClusterPoints;
			pointsCreated += clusterPoints;
		}

		result.swap(pointSet);
	}

	template <int N, typename Real>
	void generateUniformCubePointSet(integer points,
						 integer dimension,
						 std::vector<Point<N, Real> >& pointSet)
	{
		ENSURE1(points > 0, points);
		ENSURE1(dimension > 0, dimension);
		ENSURE2(N == Dynamic || N == dimension, N, dimension);

		std::vector<Point<N, Real> > result;
		result.reserve(points);

		for (integer i = 0;i < points;++i)
		{
			result.push_back(
				asPoint(randomVectorCube<N, Real>(dimension)));
		}

		result.swap(pointSet);
	}

	template <int N, typename Real>
	void generateGaussianPointSet(integer points,
						 integer dimension,
						 std::vector<Point<N, Real> >& pointSet)
	{
		ENSURE1(points > 0, points);
		ENSURE1(dimension > 0, dimension);
		ENSURE2(N == Dynamic || N == dimension, N, dimension);

		std::vector<Point<N, Real> > result;
		result.reserve(points);

		for (integer i = 0;i < points;++i)
		{
			result.push_back(
				asPoint(randomGaussianVector<N, Real>(dimension)));
		}

		result.swap(pointSet);
	}

}

#endif
