#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_ALIGNEDBOX_HPP
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_ALIGNEDBOX_HPP

#include "pastel/geometry/bounding_alignedbox_alignedbox.h"

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const AlignedBox<Real, N>& aAlignedBox, 
		const AlignedBox<Real, N>& bAlignedBox)
	{
		const AlignedBox<Real, N> result(
			min(aAlignedBox.min(), bAlignedBox.min()),
			max(aAlignedBox.max(), bAlignedBox.max()));

		return result;
	}

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const AlignedBox<Real, N>& alignedBox,
		const AffineTransformation<Real>& transformation)
	{
		// Let the affine transformation be given by
		//
		// f : R^n -> R^n: f(x) = Ax + b
		// where
		// A in R^{n x n}
		// b is R^n
		//
		// Let the aligned box be given by the minimum
		// and maximum points p_min and p_max and define:
		//
		// p = (p_min + p_max) / 2
		// w = (p_max - p_min) / 2
		//
		// It is easy to see that the 'radius' of the
		// transformed aligned box in the k:th standard
		// basis axis is given by:
		//
		// r_k = sum_{i = 1}^n |dot(e_k, A ((w_i / 2) e_i)|
		// = sum_{i = 1}^n |A_ki| (w_i / 2)
		//
		// Thus:
		// r = abs(A) w / 2
		
		const Vector<Real, N> radius =
			(alignedBox.extent() * abs(transformation.matrix())) * 0.5;
		
		const Vector<Real, N> center =
			transformPoint(transformation, linear(alignedBox.min(), alignedBox.max(), 0.5));
		
		const AlignedBox<Real, N> result(
			center - radius, center + radius);

		return result;
	}

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedCube(
		const AlignedBox<Real, N>& box)
	{
		const Real maxRadius = max(box.extent()) * 0.5;
		const Vector<Real, N> center = linear(box.min(), box.max(), 0.5);
		const Vector<Real, N> minPoint = center - maxRadius;
		const Vector<Real, N> maxPoint = center + maxRadius;

		const AlignedBox<Real, N> result(
			min(minPoint, box.min()),
			max(maxPoint, box.max()));

		return result;
	}

	template <typename Real, int N>
	bool extendToCover(
		const AlignedBox<Real, N>& boxToCover,
		AlignedBox<Real, N>& boxToExtend)
	{
		bool neededToExtend = Pastel::extendToCover(boxToCover.min(),
			boxToExtend);
		neededToExtend |= Pastel::extendToCover(boxToCover.max(),
			boxToExtend);

		return neededToExtend;
	}

	template <typename Real, int N>
	bool extendToCover(
		const Vector<Real, N>& pointToCover,
		AlignedBox<Real, N>& boxToExtend)
	{
		const integer dimension = pointToCover.size();
		PENSURE_OP(dimension, ==, boxToExtend.n());

		Vector<Real, N>& min = boxToExtend.min();
		Vector<Real, N>& max = boxToExtend.max();

		bool neededToExtend = false;

		for (integer i = 0;i < dimension;++i)
		{
			if (pointToCover[i] < min[i])
			{
				min[i] = pointToCover[i];
				neededToExtend = true;
			}
			if (pointToCover[i] > max[i])
			{
				max[i] = pointToCover[i];
				neededToExtend = true;
			}
		}

		return neededToExtend;
	}

}

#endif
