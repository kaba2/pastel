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
		Minkowksi_NormBijection<Real> normBijection(metric);
		
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
		PENSURE_OP(aPoint.dimension(), ==, bPoint.dimension());

		Tuple<Real, N>::const_iterator aData = aPoint.asTuple().begin();
		const Tuple<Real, N>::const_iterator aEnd = aPoint.asTuple().end();
		Tuple<Real, N>::const_iterator bData = bPoint.asTuple().begin();

		Real result = 0;
		while(aData != aEnd)
		{
			result = normBijection.addAxis(
				result, 
				normBijection.signedAxis(*aData - *bData));

			++aData;
			++bData;
		}

		return result;
	}

	template <typename Real, typename NormBijection>
	Real distance2(
		const Real* aPoint,
		const Real* bPoint,
		integer dimension,
		const NormBijection& normBijection)
	{
		PENSURE_OP(dimension, >=, 0);

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

	template <typename Real, int N, typename NormBijection>
	Real distance2(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint,
		const NormBijection& normBijection,
		const PASTEL_NO_DEDUCTION(Real)& cullDistance)
	{
		PENSURE_OP(aPoint.dimension(), ==, bPoint.dimension());

		Tuple<Real, N>::const_iterator aData = aPoint.asTuple().begin();
		const Tuple<Real, N>::const_iterator aEnd = aPoint.asTuple().end();
		Tuple<Real, N>::const_iterator bData = bPoint.asTuple().begin();

		Real result = 0;
		while(aData != aEnd)
		{
			result = normBijection.addAxis(
				result, 
				normBijection.signedAxis(*aData - *bData));

			if (result > cullDistance)
			{
				break;
			}

			++aData;
			++bData;
		}

		return result;
	}

}

#endif
