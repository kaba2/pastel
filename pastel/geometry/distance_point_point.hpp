#ifndef PASTEL_DISTANCE_POINT_POINT_HPP
#define PASTEL_DISTANCE_POINT_POINT_HPP

#include "pastel/geometry/distance_point_point.h"

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
		const Manhattan_NormBijection<Real> normBijection;
		
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
		const Euclidean_NormBijection<Real> normBijection;
		
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
		const Minkowksi_NormBijection<Real> 
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
		const Maximum_NormBijection<Real> normBijection;
		
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
		const Euclidean_NormBijection<Real> normBijection;

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
		PENSURE_OP(aPoint.dimension(), ==, bPoint.dimension());

		return Pastel::distance2(
			aPoint.rawBegin(),
			bPoint.rawBegin(),
			aPoint.dimension(),
			normBijection);
	}

	template <typename Real, typename NormBijection>
	Real distance2(
		const Real* aPoint,
		const Real* bPoint,
		integer dimension,
		const NormBijection& normBijection)
	{
		PENSURE_OP(dimension, >=, 0);

		const Real* aEnd = aPoint + dimension;

		Real result = 0;
		while(aPoint != aEnd)
		{
			result = normBijection.addAxis(
				result, 
				normBijection.signedAxis(*aPoint - *bPoint));

			++aPoint;
			++bPoint;
		}

		return result;
	}

	template <typename Real, typename NormBijection>
	Real distance2(
		const Real* aPoint,
		const Real* bPoint,
		integer dimension,
		const NormBijection& normBijection,
		const Real& cullDistance)
	{
		PENSURE_OP(dimension, >=, 0);

		const Real* aEnd = aPoint + dimension;

		Real result = 0;
		while(aPoint != aEnd)
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
		PENSURE_OP(aPoint.dimension(), ==, bPoint.dimension());

		return Pastel::distance2(
			aPoint.rawBegin(),
			bPoint.rawBegin(),
			aPoint.size(),
			normBijection,
			cullDistance);
	}

}

#endif
