#ifndef PASTEL_BOUNDING_ALIGNEDBOX_HPP
#define PASTEL_BOUNDING_ALIGNEDBOX_HPP

#include "pastel/geometry/bounding_alignedbox.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

#include "pastel/sys/constants.h"

namespace Pastel
{

	template <int N, typename Real, typename InputIterator>
	AlignedBox<Real, N> boundingAlignedBox(
		integer dimension,
		const InputIterator& from,
		const InputIterator& to)
	{
		PENSURE1((N == Dynamic && dimension > 0) ||
			(N != Dynamic && dimension == N), dimension);

		AlignedBox<Real, N> result(dimension);

		if (from != to)
		{
			PENSURE_OP(dimension, ==, from->dimension());

			InputIterator iter(from);
			while (iter != to)
			{
				extendToCover(*iter, result);

				++iter;
			}
		}

		return result;
	}

	template <int N, typename Real>
	AlignedBox<Real, N> boundingAlignedBox(
		const AlignedBox<Real, N>& aAlignedBox, 
		const AlignedBox<Real, N>& bAlignedBox)
	{
		return AlignedBox<Real, N>(
			min(aAlignedBox.min(), bAlignedBox.min()),
			max(aAlignedBox.max(), bAlignedBox.max()));
	}

	template <int N, typename Real>
	AlignedBox<Real, N> boundingAlignedBox(
		const Sphere<Real, N>& sphere)
	{
		return AlignedBox<Real, N>(
			sphere.position() - Vector<Real, N>(sphere.radius()),
			sphere.position() + Vector<Real, N>(sphere.radius()));
	}

	template <int N, typename Real>
	AlignedBox<Real, N> boundingAlignedBox(
		const Box<Real, N>& box)
	{
		const integer dimension = box.dimension();

		const Vector<Real, N>& width = box.width();
		Vector<Real, N> radius(ofDimension(dimension));

		for (integer i = 0;i < dimension;++i)
		{
			Real axisRadius = 0;
			for (integer k = 0;k < dimension;++k)
			{
				// Actually, this is
				// mabs(dot(box.rotation()[k] * width[k], e_i))
				// where e_i is the i:th standard basis vector.
				axisRadius += mabs(box.rotation()(k, i) * width[k]);
			}

			radius[i] = axisRadius;
		}

		return AlignedBox<Real, N>(
			box.position() - radius, 
			box.position() + radius);
	}

	template <int N, typename Real>
	AlignedBox<Real, N> boundingAlignedBox(
		const Segment<Real, N>& segment)
	{
		return AlignedBox<Real, N>(
			min(segment.start(), segment.end()),
			max(segment.start(), segment.end()));
	}

	template <int N, typename Real, int M>
	AlignedBox<Real, N> boundingAlignedBox(
		const Simplex<Real, N, M>& simplex)
	{
		return Pastel::boundingAlignedBox<N, Real>(
			simplex.begin(), simplex.end());
	}

	template <int N, typename Real>
	AlignedBox<Real, N> boundingAlignedCube(
		const AlignedBox<Real, N>& box)
	{
		const Real maxRadius = max(box.extent()) * 0.5;
		const Vector<Real, N> center = linear(box.min(), box.max(), 0.5);
		Vector<Real, N> minPoint = center - maxRadius;
		Vector<Real, N> maxPoint = center + maxRadius;

		return AlignedBox<Real, N>(
			min(minPoint, box.min()),
			max(maxPoint, box.max()));
	}

	template <int N, typename Real>
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

	template <int N, typename Real>
	bool extendToCover(
		const Vector<Real, N>& pointToCover,
		AlignedBox<Real, N>& boxToExtend)
	{
		const integer dimension = pointToCover.size();
		PENSURE_OP(dimension, ==, boxToExtend.dimension());

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
			else if (pointToCover[i] > max[i])
			{
				max[i] = pointToCover[i];
				neededToExtend = true;
			}
		}

		return neededToExtend;
	}

}

#endif
