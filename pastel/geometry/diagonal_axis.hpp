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
				return square(data_[right]) < square(data_[left]);
			}

		private:
			const Vector<N, Real>& data_;
		};

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

		Real sum = 0;
		Real maximumFit = -infinity<Real>();
		DiagonalAxis currentAxis = 0;
		DiagonalAxis bestAxis = 0;

		for (integer i = 0;i < dimension;++i)
		{
			const integer index = permutation[i];
			const Real value = that[index];

			if (value >= 0)
			{
				currentAxis += 1 << (2 * index);
				sum += value;
			}
			else
			{
				currentAxis += 3 << (2 * index);
				sum -= value;
			}

			const Real fit = square(sum) / (i + 1);
			if (fit > maximumFit)
			{
				maximumFit = fit;
				bestAxis = currentAxis;
			}
		}

		return bestAxis;
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
