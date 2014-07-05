#ifndef PASTELGEOMETRY_DISTANCE_POINT_POINT_HPP
#define PASTELGEOMETRY_DISTANCE_POINT_POINT_HPP

#include "pastel/geometry/distance_point_point.h"

#include "pastel/math/normbijections.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Real distance(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint)
	{
		return euclideanDistance(aPoint, bPoint);
	}

	template <typename Real, int N>
	Real manhattanDistance(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint)
	{
		Manhattan_NormBijection<Real> normBijection;
		
		return normBijection.toNorm(
			Pastel::distance2(
			aPoint, bPoint,
			normBijection));
	}

	template <typename Real, int N>
	Real euclideanDistance(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint)
	{
		Euclidean_NormBijection<Real> normBijection;
		
		return normBijection.toNorm(
			Pastel::distance2(
			aPoint, bPoint,
			normBijection));
	}

	template <typename Real, int N>
	Real minkowskiDistance(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint,
		const PASTEL_NO_DEDUCTION(Real)& metric)
	{
		Minkowski_NormBijection<Real> 
			normBijection(metric);
		
		return normBijection.toNorm(
			Pastel::distance2(
			aPoint, bPoint,
			normBijection));
	}

	template <typename Real, int N>
	Real maximumDistance(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint)
	{
		Maximum_NormBijection<Real> normBijection;
		
		return normBijection.toNorm(
			Pastel::distance2(
			aPoint, bPoint,
			normBijection));
	}

	template <typename Real, int N>
	Real distance2(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint)
	{
		Euclidean_NormBijection<Real> normBijection;

		return Pastel::distance2(
			aPoint, bPoint,
			normBijection);
	}

	template <typename Real, int N, typename NormBijection>
	Real distance2(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint,
		const NormBijection& normBijection)
	{
		PENSURE_OP(aPoint.n(), ==, bPoint.n());

		return Pastel::distance2(
			aPoint.rawBegin(),
			bPoint.rawBegin(),
			aPoint.n(),
			normBijection);
	}

	template <
		typename Real_ConstIterator_A,
		typename Real_ConstIterator_B, 
		typename NormBijection>
	typename NormBijection::Real distance2(
		Real_ConstIterator_A aPoint,
		Real_ConstIterator_B bPoint,
		integer dimension,
		const NormBijection& normBijection)
	{
		PENSURE_OP(dimension, >=, 0);

		using Real = typename NormBijection::Real;

		Real result = 0;
		for (integer i = 0;i < dimension;++i)
		{
			result = normBijection.addAxis(
				result, 
				normBijection.signedAxis(*aPoint - *bPoint));

			++aPoint;
			++bPoint;
		}

		return result;
	}

	template <
		typename Real_ConstIterator_A,
		typename Real_ConstIterator_B, 
		typename NormBijection>
	typename NormBijection::Real distance2(
		Real_ConstIterator_A aPoint,
		Real_ConstIterator_B bPoint,
		integer dimension,
		const NormBijection& normBijection,
		const typename NormBijection::Real& cullDistance)
	{
		PENSURE_OP(dimension, >=, 0);

		using Real = typename NormBijection::Real;

		Real result = 0;
		for (integer i = 0;i < dimension;++i)
		{
			result = normBijection.addAxis(
				result, 
				normBijection.signedAxis(*aPoint - *bPoint));
			if (result > cullDistance)
			{
				break;
			}

			++aPoint;
			++bPoint;
		}

		return result;
	}

	template <typename Real, int N, typename NormBijection>
	Real distance2(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint,
		const NormBijection& normBijection,
		const PASTEL_NO_DEDUCTION(Real)& cullDistance)
	{
		PENSURE_OP(aPoint.n(), ==, bPoint.n());

		return Pastel::distance2(
			aPoint.rawBegin(),
			bPoint.rawBegin(),
			aPoint.size(),
			normBijection,
			cullDistance);
	}

}

#endif
