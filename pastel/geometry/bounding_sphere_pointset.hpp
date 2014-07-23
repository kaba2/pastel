#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_HPP
#define PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_HPP

#include "pastel/geometry/bounding_sphere_pointset.h"

#include "pastel/math/statistics.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Sphere<Real, N> boundingSphere(
		const Vector<Real, N>& aPoint)
	{
		return Sphere<Real, N>(aPoint, 0);
	}

	template <typename Real, int N>
	Sphere<Real, N> boundingSphere(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint)
	{
		return Sphere<Real, N>(
			linear(aPoint, bPoint, 0.5),
			norm(evaluate(bPoint - aPoint)) * 0.5);
	}

	template <typename Point_ConstRange, typename Locator>
	Sphere<typename Locator::Real, Locator::N> 
		boundingSphere(
		const Point_ConstRange& pointSet,
		const Locator& locator)
	{
		using Real = typename Locator::Real;
		static PASTEL_CONSTEXPR int N = Locator::N;

		// This function does not give the minimum volume
		// bounding sphere, but it does give something to
		// that direction, and is fast.

		integer n = locator.n();
		ENSURE(n != Dynamic);

		Sphere<Real, N> result(n);
		if (pointSet.empty())
		{
			return result;
		}

		// Compute the midpoint.
		const Vector<Real, N> midPoint = 
			pointSetMean(pointSet, locator);

		// Compute the maximum distance from the midpoint.
		Real maxDistance2 = 0;
		auto iter = pointSet.begin();
		auto iterEnd = pointSet.end();
		while(iter != iterEnd)
		{
			const Real currentDistance2 =

				dot(pointAsVector(*iter, locator) - midPoint);
			if (currentDistance2 > maxDistance2)
			{
				maxDistance2 = currentDistance2;
			}

			++iter;
		}

		return Sphere<Real, N>(
			midPoint,
			std::sqrt(maxDistance2));
	}

}

#endif
