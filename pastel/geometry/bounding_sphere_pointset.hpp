#ifndef PASTEL_BOUNDING_SPHERE_POINTSET_HPP
#define PASTEL_BOUNDING_SPHERE_POINTSET_HPP

#include "pastel/geometry/bounding_sphere_pointset.h"

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

	template <typename Real, int N, typename InputIterator, typename PositionFunctor>
	Sphere<Real, N> boundingSphere(
		const InputIterator& from,
		const InputIterator& to,
		const PositionFunctor& positionFunctor)
	{
		// This does not give the minimum volume
		// bounding sphere, but it does give something.

		if (from == to)
		{
			return Sphere<Real, N>();
		}

		Vector<Real, N> midPoint(0);

		integer points = 0;

		// Find out the midpoint.

		InputIterator iter = from;
		while(iter != to)
		{
			midPoint += positionFunctor(*iter);
			++iter;
			++points;
		}

		midPoint /= points;

		// Compute the maximum distance from the midpoint.

		Real maxDistance2 = 0;

		iter = from;
		while(iter != to)
		{
			const Real currentDistance2 =
				dot(positionFunctor(*iter) - midPoint);
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

	namespace Detail_BoundingSphere
	{

		template <typename Real, int N>
		class PositionFunctor
		{
		public:
			const Vector<Real, N>& operator()(const Vector<Real, N>& position) const
			{
				return position;
			}
		};

	}

	template <typename Real, int N, typename InputIterator>
	Sphere<Real, N> boundingSphere(
		const InputIterator& from,
		const InputIterator& to)
	{
		Detail_BoundingSphere::PositionFunctor<Real, N> positionFunctor;
		return Pastel::boundingSphere<Real, N>(from, to, positionFunctor);
	}

}

#endif
