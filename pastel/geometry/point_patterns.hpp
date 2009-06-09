#ifndef PASTEL_POINT_PATTERNS_HPP
#define PASTEL_POINT_PATTERNS_HPP

#include "pastel/geometry/point_patterns.h"

#include "pastel/sys/random.h"

#include "pastel/math/uniformsampling.h"
#include "pastel/math/matrix.h"
#include "pastel/math/orthonormal.h"

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

	template <int Width, int Height, typename Real>
	void setRandomRotation(
		Matrix<Height, Width, Real>& result)
	{
		const integer height = result.height();
		const integer width = result.width();

		ENSURE2(height <= width, height, width);

		std::vector<Vector<Width, Real> > orthonormalSet;
		orthonormalSet.reserve(height);
		
		orthonormalSet.push_back(
			randomVectorBall<Width, Real>(width));
		result[0] = orthonormalSet.back();
		
		for (integer i = 1;i < height;++i)
		{
			orthonormalSet.push_back(
				perpendicular(orthonormalSet));
			result[i] = orthonormalSet.back();
		}
	}

	template <int N, typename Real>
	void generateGaussianEllipsoidPointSet(
		integer points,
		integer dimension,
		std::vector<Point<N, Real> >& pointSet)
	{
		ENSURE1(points > 0, points);
		ENSURE1(dimension > 0, dimension);
		ENSURE2(N == Dynamic || N == dimension, N, dimension);

		Vector<N, Real> scaling = randomVector<N, Real>(dimension);

		Matrix<N, N, Real> rotation(dimension, dimension);
		setRandomRotation(rotation);

		std::vector<Point<N, Real> > result;
		result.reserve(points);

		for (integer i = 0;i < points;++i)
		{
			result.push_back(
				asPoint(
				evaluate(randomGaussianVector<N, Real>(dimension) * scaling) * rotation));
		}

		result.swap(pointSet);
	}

}

#endif
