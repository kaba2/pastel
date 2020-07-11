// Description: Distance between two aligned boxes

#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/shape/alignedbox.h"

#include "pastel/math/norm/norm_concept.h"
#include "pastel/math/norm/euclidean_norm.h"

#include "pastel/geometry/distance/distance_alignedbox_alignedbox.h"
#include "pastel/geometry/distance/distance_point_point.h"

namespace Pastel
{

	//! Computes the norm-bijection distance between two aligned boxes.
	/*!
	Preconditions:
	aBox.n() == bBox.n()

	Time complexity: O(aBox.n())

	The distance is computed assuming that the boxes are solid. 
	*/
	template <
		typename Real, int N, 
		Norm_Concept Norm = Euclidean_Norm<Real>
	>
	auto distance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const Norm& norm = Norm())
	{
		PENSURE_OP(aBox.n(), ==, bBox.n());

		auto distance = norm();

		integer n = aBox.n();
		for (integer i = 0;i < n;++i)
		{
			const Real& aMin = aBox.min()[i];
			const Real& aMax = aBox.max()[i];
			const Real& bMin = bBox.min()[i];
			const Real& bMax = bBox.max()[i];

			if (aMin > bMax)
			{
				distance.set(i, aMin - bMax);
			}
			else if (bMin > aMax)
			{
				distance.set(i, bMin - aMax);
			}
			// else the projection intervals overlap.
		}

		return distance;
	}

	//! Computes the farthest norm-bijection distance between two aligned boxes.
	/*!
	Preconditions:
	aBox.n() == bBox.n()

	Time complexity: O(aBox.n())

	The distance is computed assuming that the boxes are solid. 
	*/
	template <
		typename Real, int N, 
		Norm_Concept Norm = Euclidean_Norm<Real>
	>
	auto farthestDistance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const Norm& norm = Norm())
	{
		PENSURE_OP(aBox.n(), ==, bBox.n());

		auto distance = norm();

		integer n = aBox.n();
		for (integer i = 0;i < n;++i)
		{
			const Real& aMin = aBox.min()[i];
			const Real& aMax = aBox.max()[i];
			const Real& bMin = bBox.min()[i];
			const Real& bMax = bBox.max()[i];

			if (aMin > bMin)
			{
				if (bMax > aMax)
				{
					distance.set(i, std::max(bMax - aMin, aMax - bMin));
				}
				else
				{
					distance.set(i, aMax - bMin);
				}
			}
			else 
			{
				if (bMax > aMax)
				{
					distance.set(i, bMax - aMin);
				}
				else
				{
					distance.set(i, std::max(bMax - aMin, aMax - bMin));
				}				
			}
		}

		return distance;
	}

}

#endif
