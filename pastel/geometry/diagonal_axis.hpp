#ifndef PASTEL_DIAGONAL_AXIS_HPP
#define PASTEL_DIAGONAL_AXIS_HPP

#include "pastel/geometry/diagonal_axis.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	namespace Detail_DiagonalAxis
	{

		template <int N, typename Real>
		class Predicate
		{
		public:
			explicit Predicate(
				const Vector<N, Real>& data)
				: data_(data)
			{
			}

			bool operator()(integer left, integer right) const
			{
				return data_[right] < data_[left];
			}

		private:
			const Vector<N, Real>& data_;
		};

	}

	template <int N, typename Real>
	TemporaryPoint<N, Real> mean(
		const std::vector<Point<N, Real> >& pointSet)
	{
		const integer points = pointSet.size();

		if (points == 0)
		{
			Point<N, Real> result(ofDimension(0));
			return result.asTemporary();
		}

		const integer dimension = pointSet.front().dimension();

		Point<N, Real> result(ofDimension(dimension), 0);
		for (integer i = 0;i < points;++i)
		{
			result += asVector(pointSet[i]);
		}

		asVector(result) /= points;
		
		return result.asTemporary();
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> axisAlignedVariance(
		const std::vector<Point<N, Real> >& pointSet,
		const Point<N, Real>& mean)
	{
		const integer points = pointSet.size();
		const integer dimension = pointSet.front().dimension();

		Vector<N, Real> result(ofDimension(dimension));
		for (integer i = 0;i < points;++i)
		{
			result += squarev(pointSet[i] - mean);
		}
		
		return result.asTemporary();
	}

	template <int N, typename Real>
	DiagonalAxis maximalDiagonalVariance(
		const std::vector<Point<N, Real> >& pointSet)
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N <= 16);

		if (pointSet.empty())
		{
			return 1;
		}

		const integer points = pointSet.size();
		const integer dimension = pointSet.front().dimension();
		
		ENSURE1(N != Dynamic || dimension <= 16, dimension);

		const Point<N, Real> meanPoint = mean(pointSet);
		const Vector<N, Real> axisVariance = axisAlignedVariance(pointSet, meanPoint);

		std::vector<integer> permutation;
		permutation.reserve(dimension);
		for (integer j = 0;j < dimension;++j)
		{
			permutation.push_back(j);
		}

		const Detail_DiagonalAxis::Predicate<N, Real> 
			predicate(axisVariance);

		std::sort(permutation.begin(),
			permutation.end(), predicate);
		
		const integer firstAxis = permutation.front();

		// dotProductSet[i] = <v[i], f_1>

		std::vector<Real> dotProductSet;
		dotProductSet.reserve(points);
		for (integer i = 0;i < points;++i)
		{
			dotProductSet.push_back(pointSet[i][firstAxis] - meanPoint[firstAxis]);
		}

		Real variance = axisVariance[firstAxis];
		DiagonalAxis result = 1 << (firstAxis * 2);

		Real resultNorm2 = 1;

		for (integer j = 1;j < dimension;++j)
		{
			const integer axis = permutation[j];

			Real sum = 0;
			for (integer i = 0;i < points;++i)
			{
				sum += dotProductSet[i] * (pointSet[i][axis] - meanPoint[axis]);
			}

			const Real varianceDelta =
				axisVariance[axis] + 2 * mabs(sum);

			if (variance < resultNorm2 * varianceDelta)
			{
				if (sum >= 0)
				{
					result += 1 << (axis * 2);
					for (integer i = 0;i < points;++i)
					{
						dotProductSet[i] += (pointSet[i][axis] - meanPoint[axis]);
					}
				}
				else
				{
					result += 3 << (axis * 2);
					for (integer i = 0;i < points;++i)
					{
						dotProductSet[i] -= (pointSet[i][axis] - meanPoint[axis]);
					}
				}

				variance += varianceDelta;
				++resultNorm2;
			}
		}

		return result;
	}

	template <int N, typename Real>
	DiagonalAxis nearestDiagonalAxis(
		const Vector<N, Real>& that)
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N <= 32);

		const integer dimension = that.dimension();

		PENSURE1(dimension <= 32, dimension);
		PENSURE(allGreaterEqual(that, 0));

		std::vector<integer> permutation;
		permutation.reserve(dimension);
		for (integer i = 0;i < dimension;++i)
		{
			permutation.push_back(i);
		}

		const Detail_DiagonalAxis::Predicate<N, Real> 
			predicate(that);

		std::sort(permutation.begin(),
			permutation.end(), predicate);

		Real sum = that[permutation.front()];
		Real minimumCandidate2 = square(sum);
		integer minimumCandidate2Index = 0;

		for (integer i = 1;i < dimension;++i)
		{
			sum += that[permutation[i]];
			Real candidate2 = square(sum) / (i + 1);
			if (candidate2 < minimumCandidate2)
			{
				minimumCandidate2 = candidate2;
				minimumCandidateIndex = i;
			}
		}
		
		DiagonalAxis result = 0;
		for (integer i = 0;i < minimumCandidateIndex + 1;++i)
		{
			result |= (1 << permutation[i]);
		}

		return result;
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> diagonalAxis(
		integer dimension,
		DiagonalAxis diagonal)
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N <= 16);
		ENSURE1(N != Dynamic || dimension <= 16, dimension);

		Vector<N, Real> result(ofDimension(dimension));
		for (integer i = 0;i < dimension;++i)
		{
			const integer axis = (diagonal >> (2 * i)) & 0x3;
			if (axis == 3)
			{
				result[i] = -1;
			}
			else if (axis == 1)
			{
				result[i] = 1;
			}
		}

		return result.asTemporary();
	}

}

#endif
